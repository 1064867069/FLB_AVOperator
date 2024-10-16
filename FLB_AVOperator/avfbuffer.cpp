#include "avoperator.h"
#include "avreader.h"

#include <QDebug>

FAVFrameBuffer::FAVFrameBuffer(size_t cap) :m_limCapacity(cap)
{
	if (m_limCapacity < 10)
		m_limCapacity = 10;
}

float FAVFrameBuffer::fullProp()const
{
	return static_cast<float>(m_bufferFrames.size()) / m_limCapacity;
}

bool FAVFrameBuffer::isBeyond()const
{
	return m_bufferFrames.size() >= m_limCapacity;
}

bool FAVFrameBuffer::isEmpty()const
{
	return m_bufferFrames.size() == 0;
}

void FAVFrameBuffer::waitNotBeyond()const
{
	if (this->isBeyond())
	{
		QMutexLocker locker(&m_mutex);
		if (this->isBeyond())
		{
			m_condNotBeyond.wait(&m_mutex, 5);
		}
	}
}

void FAVFrameBuffer::pushFrame(const FrameSPtr& sp)
{
	if (sp->valid())
	{
		QMutexLocker locker(&m_mutex);
		m_bufferFrames.append(sp);
		/*while (m_bufferFrames.size() > m_limCapacity * 2)
		{
			m_bufferFrames.pop_front();
			if (m_curIndex > 0)
				m_curIndex--;
		}*/
	}
}

FrameSPtr FAVFrameBuffer::popFrame()
{
	QMutexLocker locker(&m_mutex);

	/*if (m_curIndex >= m_bufferFrames.size())
		return nullptr;

	size_t mid = m_bufferFrames.size() / 2;
	if (m_curIndex < mid)
	{
		return m_bufferFrames[m_curIndex++];
	}
	else
	{
		FrameSPtr res = m_bufferFrames[m_curIndex];
		m_bufferFrames.pop_front();

		if (m_bufferFrames.size() == m_limCapacity - 1)
		{
			m_condNotBeyond.wakeAll();
		}

		return res;
	}*/

	if (this->isEmpty())
		return nullptr;

	auto res = m_bufferFrames.front();
	m_bufferFrames.pop_front();
	return res;
}

FrameSPtr FAVFrameBuffer::frontFrame()const
{
	return m_bufferFrames.size() == 0 ? nullptr : m_bufferFrames.front();
}

void FAVFrameBuffer::clear()
{
	QMutexLocker locker(&m_mutex);
	m_bufferFrames.clear();
	m_condNotBeyond.notify_all();
}
