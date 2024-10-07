#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>

class ThreadPool {
public:
	static ThreadPool& getInstance() {
		static ThreadPool ls_instance(8);
		return ls_instance;
	}

	// 添加新任务到任务队列，并返回一个 future 对象以获取结果
	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args)
		->std::future<typename std::result_of<F(Args...)>::type>;

private:
	ThreadPool(size_t numThreads);
	~ThreadPool();

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool* operator=(const ThreadPool&) = delete;

	// 工作线程的函数
	void worker();

	// 线程池
	std::vector<std::thread> workers;

	// 任务队列
	std::queue<std::function<void()>> tasks;

	// 同步和任务处理控制
	std::mutex queueMutex;
	std::condition_variable condition;
	std::atomic<bool> stop;
};



template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type> {
	using returnType = typename std::result_of<F(Args...)>::type;

	auto task = std::make_shared<std::packaged_task<returnType()>>(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

	std::future<returnType> res = task->get_future();
	{
		std::unique_lock<std::mutex> lock(queueMutex);

		// 不允许在线程池停止时添加任务
		if (stop.load())
			throw std::runtime_error("enqueue on stopped ThreadPool");

		tasks.emplace([task]() { (*task)(); });
	}
	condition.notify_one();
	return res;
}


#endif // !THREADPOOL_H

