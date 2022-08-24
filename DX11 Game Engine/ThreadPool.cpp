#include "ThreadPool.h"

ThreadPool::ThreadPool() : isComplete(false) {
	const unsigned int maxThreads = std::thread::hardware_concurrency() / 2;

	try {
		for (int i = 0; i < maxThreads; i++) {
			threads.push_back(std::thread(&ThreadPool::executeTask, this));
		}
	} catch (...) {
		isComplete = true;
		throw;
	}
}

ThreadPool::~ThreadPool() {
	isComplete = true;
}

void ThreadPool::executeTask() {
	while (!isComplete) {
		std::function<void()> task;
		if (taskCount > 0) {
			queueLock.lock();
			task = tasks.front();
			tasks.pop();
			queueLock.unlock();
			taskCount--;
			task();
		} else {
			std::this_thread::yield();
		}
	}
}
