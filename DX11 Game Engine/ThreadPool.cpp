#include "ThreadPool.h"

ThreadPool::ThreadPool() : isComplete(false) {
	const unsigned int maxThreads = 4;

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
	lock1.unlock();
	lock2.unlock();
}

void ThreadPool::executeTask() {
	while (!isComplete) {
		std::function<void()> task;
		if (tasks.size() > 0) {
			lock2.lock();
			task = tasks.front();
			tasks.pop();
			lock2.unlock();
			task();
		} else {
			std::this_thread::yield();
		}
	}
}
