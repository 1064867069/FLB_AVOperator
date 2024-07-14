#include "avoperator.h"

#include <QDebug>
#include <qlogging.h>

#define MAX_AUDIO_FRAME_SIZE 10240 // 一帧音频最大长度（样本数），该值不能太小

AudioSDLPlayer::AudioSDLPlayer(QObject* p) :QObject(p)
{

}


AudioSDLPlayer::~AudioSDLPlayer()
{
	reset();
}

AudioSDLPlayer* AudioSDLPlayer::getInstance()
{
	static AudioSDLPlayer ls_player;
	return &ls_player;
}

void AudioSDLPlayer::pause()
{
	if (m_spReader)
		SDL_PauseAudio(1);
}

void AudioSDLPlayer::cont()
{
	if (m_spReader)
		SDL_PauseAudio(0);
}

void AudioSDLPlayer::resetNoLock()
{
	if (m_spReader)
	{
		SDL_CloseAudio();
		m_spReader->stop();

		if (m_pSwrCtx != nullptr)
		{
			swr_free(&m_pSwrCtx);
		}

		if (m_pOutBuffer != nullptr)
		{
			av_free(m_pOutBuffer);
			m_pOutBuffer = nullptr;
		}

		m_spReader.reset();
	}

	m_pBindPlayer = nullptr;
}

void AudioSDLPlayer::reset()
{
	QMutexLocker locker(&m_mutex);
	this->resetNoLock();
}

bool AudioSDLPlayer::changeReader(ReaderSPtr sp)
{
	if (!sp)
	{
		this->reset();
		return false;
	}

	auto info = sp->getInfo();
	if (info->m_aIndx < 0)
	{
		this->reset();
		return false;
	}

	// QMutexLocker locker(&m_mutex);

	if (m_spReader)
	{
		if (info == m_spReader->getInfo() || info->equalAudioInfo(*m_spReader->getInfo()))
		{
			m_spReader = std::move(sp);
			return true;
		}
	}

	SDL_CloseAudio();

	AVChannelLayout oly = info->m_chLayout;
	AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16; // 输出的采样格式
	int ret = swr_alloc_set_opts2(&m_pSwrCtx, // 音频采样器的实例
		&oly, // 输出的声道布局
		out_sample_fmt, // 输出的采样格式
		info->m_sampleRate, // 输出的采样频率
		&oly, // 输入的声道布局
		info->m_sampleFmt, // 输入的采样格式
		info->m_sampleRate, // 输入的采样频率
		0, nullptr);
	if (ret < 0) {
		qCritical() << "swr_alloc_set_opts2 error " << ret << '\n';
		return false;
	}

	swr_init(m_pSwrCtx); // 初始化音频采样器的实例
	if (ret < 0) {
		qCritical() << "swr_init error " << ret << '\n';
		swr_free(&m_pSwrCtx);
		return false;
	}

	m_outBufferSize = av_samples_get_buffer_size(NULL, info->m_nChannel, info->m_frameSize, out_sample_fmt, 1);
	// 分配输出缓冲区的空间
	m_pOutBuffer = (unsigned char*)av_malloc(MAX_AUDIO_FRAME_SIZE * info->m_nChannel);
	if (m_pOutBuffer == nullptr)
	{
		qCritical() << "outbuffer alloc error " << ret << '\n';
		swr_free(&m_pSwrCtx);
		return false;
	}

	m_spec.freq = info->m_sampleRate; // 采样频率
	m_spec.format = AUDIO_S16SYS; // 采样格式
	m_spec.channels = info->m_nChannel; // 声道数量
	m_spec.silence = 0; // 是否静音
	m_spec.samples = info->m_frameSize; // 采样数量
	m_spec.callback = fill_audio; // 回调函数的名称
	m_spec.userdata = nullptr; // 回调函数的额外信息，如果没有额外信息就填NULL
	if (SDL_OpenAudio(&m_spec, nullptr) < 0) { // 打开扬声器
		qCritical() << "open audio occur error" << '\n';
		this->resetNoLock();
		return false;
	}
	m_spReader = std::move(sp);
	SDL_PauseAudio(0); // 播放/暂停音频。参数为0表示播放，为1表示暂停
	return true;
}

void AudioSDLPlayer::unbindReader(ReaderSPtr sp)
{
	//QMutexLocker locker(&m_mutex);
	if (sp && m_spReader && m_spReader->getInfo() == sp->getInfo())
	{
		this->resetNoLock();
	}
}

void AudioSDLPlayer::bindAVPlayer(FAVPlayer* pPlayer)
{
	if (pPlayer == m_pBindPlayer)
		return;

	if (pPlayer == nullptr || pPlayer->m_spReader->getInfo()->m_aIndx < 0)
	{
		this->reset();
		return;
	}

	QMutexLocker locker(&m_mutex);

	if (m_pBindPlayer != nullptr)
	{
		m_pBindPlayer->pause(true);
	}
	if (this->changeReader(pPlayer->m_spReader))
		m_pBindPlayer = pPlayer;
	else
		m_pBindPlayer = nullptr;
}

void AudioSDLPlayer::unBindAVPlayer(FAVPlayer* pPlayer)
{
	QMutexLocker locker(&m_mutex);
	if (m_pBindPlayer != pPlayer)
	{
		return;
	}

	this->pause();
	m_pBindPlayer = nullptr;
	this->unbindReader(pPlayer->m_spReader);
}

void AudioSDLPlayer::fill_audio(void* para, uint8_t* stream, int len)
{
	static QList<uint8_t> ls_buffer;
	SDL_memset(stream, 0, len); // 将缓冲区清零

	auto* player = getInstance();
	QMutexLocker locker(&player->m_mutex);
	if (!player->m_spReader)
	{
		ls_buffer.clear();
		return;
	}

	if (ls_buffer.size() > 0)
	{
		std::vector<uint8_t> temp;
		while (ls_buffer.size() > 1 && len > temp.size())
		{
			temp.push_back(ls_buffer.front());
			temp.push_back(ls_buffer[1]);
			/**stream = ls_buffer.front();
			++stream;
			--len;*/
			ls_buffer.pop_front();
			ls_buffer.pop_front();
		}


		SDL_MixAudio(stream, &temp[0], temp.size(), SDL_MIX_MAXVOLUME);

		len -= temp.size();
		stream += temp.size();
	}
	if (len <= 0)
		return;

	auto reader = player->m_spReader;
	if (!reader)
	{
		return;
	}

	double curSecond = -1;
	while (len > 0 && reader->getInfo()->m_isOpen)
	{
		auto frame = reader->popAudioFrame();
		if (frame)
		{
			auto buf = player->m_pOutBuffer;
			int sz = frame->swrFrame(player->m_pSwrCtx, buf, MAX_AUDIO_FRAME_SIZE);
			sz = sz * player->m_spec.channels * av_get_bytes_per_sample(player->m_outSampleFmt);

			int fill_len = std::min(sz, len);
			//将音频数据混合到缓冲区
			SDL_MixAudio(stream, buf, fill_len, SDL_MIX_MAXVOLUME);

			buf += fill_len;
			sz -= fill_len;
			len -= fill_len;
			stream += fill_len;

			while (sz > 0)
			{
				ls_buffer.append(static_cast<uint8_t>(*buf));
				++buf;
				--sz;
			}

			curSecond = frame->getSecond();
		}

	}

	if (curSecond >= 0 && player->m_pBindPlayer != nullptr)
		player->m_pBindPlayer->setCurSecond(curSecond);
}

bool AudioSDLPlayer::audioOpen()const
{
	return m_spReader && m_spReader->getInfo()->m_isOpen;
}
