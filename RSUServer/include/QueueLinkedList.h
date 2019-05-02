#pragma once

#include <iostream>
#include "LinkedList.h"

template<class T>
class QueueLinkedList :  public LinkedList<T>
{
public:
	QueueLinkedList() = default;
	~QueueLinkedList() = default;

	T pop()
	{
		if (LinkedList<T>::isEmpty())
		{
			throw 2;
		}
		else
		{
			T firstElmnt = LinkedList<T>::firstNum();
			LinkedList<T>::removeFromFront();
			return firstElmnt;
		}
	}

	void enqueue(T value)
	{
		LinkedList<T>::insertAtBack(value);
	}

	T& front()
	{
		if (LinkedList<T>::isEmpty())
		{
			throw 3;
		}
		else
		{
			return LinkedList<T>::firstNum();
		}
	}
};
