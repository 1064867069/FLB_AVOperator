#include "threadpool.h"

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
	for (size_t i = 0; i < numThreads; ++i)
		workers.emplace_back([this] { this->worker(); });
}

ThreadPool::~ThreadPool() {
	stop.store(true);
	condition.notify_all();
	for (std::thread& worker : workers)
		worker.join();
}

void ThreadPool::worker() {
	while (!stop.load()) {
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(this->queueMutex);
			this->condition.wait(lock, [this] { return this->stop.load() || !this->tasks.empty(); });
			if (this->stop.load() && this->tasks.empty())
				return;
			task = std::move(this->tasks.front());
			this->tasks.pop();
		}
		task();
	}
}

