#pragma once

#include <vector>
#include <queue>
#include <future>
#include <mutex>
#include <atomic>
#include <functional>

template<typename T>
class ThreadRegulator {
public:
	ThreadRegulator();
	~ThreadRegulator();

	void addTask(std::function<T()> task);
	void executor(std::function<T()> task);
	void start();

private:
	bool isReady(std::future<T> const& f) {
		return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
	}

private:
	std::atomic<bool> activated;

	std::atomic<unsigned int> activeThreads;
	unsigned int maxActiveThreads;

	std::mutex threadsLock;
	std::queue<std::function<T()>> tasks;
	std::vector<std::future<T>> futures;
	std::vector<T> results;

};

