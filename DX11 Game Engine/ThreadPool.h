#pragma once

#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

class ThreadPool {
public:
	ThreadPool();
	~ThreadPool();

	void executeTask();

	template<typename func>
	void pushTask(func function) {
		queueLock.lock();
		tasks.push(std::function<void()>(function));
		queueLock.unlock();
		taskCount.store(tasks.size());
	}

private:
	std::mutex queueLock;
	std::atomic<unsigned int> taskCount = 0;
	std::atomic_bool isComplete = false;
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;

};