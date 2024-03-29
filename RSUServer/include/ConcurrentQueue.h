#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include "QueueLinkedList.h"

template <typename T>
class Queue
{
public:

	T pop()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.isEmpty())
		{
			cond_.wait(mlock);
		}
		auto val = queue_.front();
		queue_.pop();

		return val;
	}

	void push(T item)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		queue_.enqueue(item);
		mlock.unlock();
		cond_.notify_one();
	}

	Queue() = default;
	Queue(const Queue&) = delete;            // disable copying
	Queue& operator=(const Queue&) = delete; // disable assignment

private:
	QueueLinkedList<T> queue_;
	
	std::mutex mutex_;
	std::condition_variable cond_;
};