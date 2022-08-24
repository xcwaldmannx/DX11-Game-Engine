#include "ThreadRegulator.h"

#include <iostream>

template <typename T>
ThreadRegulator<T>::ThreadRegulator() {
	activated = false;
	activeThreads = 0;
	maxActiveThreads = 8;
}

template <typename T>
ThreadRegulator<T>::~ThreadRegulator() {
}

template <typename T>
void ThreadRegulator<T>::addTask(std::function<T()> task) {
	tasks.push(task);
}

template <typename T>
void ThreadRegulator<T>::executor(std::function<T()> task) {
	std::lock_guard<std::mutex> lock(threadsLock);
	activeThreads.store(activeThreads.load() + 1);
	futures.push_back(std::async(std::launch::async, task));
}

template <typename T>
void ThreadRegulator<T>::start() {
	std::lock_guard<std::mutex> lock(threadsLock);
	activated = true;
	while (activated) {
		for (int i = 0; i < futures.size(); i++) {
			if (isReady(futures[i])) {
				results.push_back(futures.begin() + i);
				futures.erase(futures.begin() + i);
				activeThreads.store(activeThreads.load() - 1);
			}
		}

		if (activeThreads < maxActiveThreads) {
			for (int i = 0; i < tasks.size(); i++) {
				executor(tasks.front());
				tasks.pop();
			}
		}

		if (tasks.empty()) {
			activated = false;
		}
	}
}
