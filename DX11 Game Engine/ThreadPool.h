#pragma once

#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>

class ThreadPool {
public:
	ThreadPool();
	~ThreadPool();

	void executeTask();

	template<typename func>
	void pushTask(func function) {
		lock1.lock();
		tasks.push(std::function<void()>(function));
		lock1.unlock();
	}

private:
	std::mutex lock1, lock2;
	bool isComplete = false;
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;

};