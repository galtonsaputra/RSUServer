#pragma once

using namespace std;
#include <iostream>

template <class T>
struct Node
{
	T val;
	Node<T> *next;
};

template <class T>
class LinkedList
{
public:
	LinkedList()
	{
		first = NULL;
		last = NULL;

	}

	~LinkedList()
	{
		Node<T>* temp = first;
		while (temp != NULL)
		{
			temp = temp->next;
			delete(first);
			first = temp;
		}

	}

	void insertAtBack(T valueToInsert)
	{
		Node<T>* newNode = new Node<T>();
		newNode->val = valueToInsert;
		newNode->next = NULL;

		Node<T>* temp = first;

		if (temp != NULL)
		{
			while (temp->next != NULL)
			{
				temp = temp->next;
			}

			temp->next = newNode;
		}
		else
		{
			first = newNode;
		}
	}

	bool removeFromBack()
	{
		if (first == NULL && last == NULL) { return false; }

		if (first == last)
		{
			cout << "First is equal to Last." << endl;
			delete(first);
			first = last = NULL;
			return true;
		}
		else
		{
			Node<T>* temp = first;
			int nodeCount = 0;

			while (temp != NULL)
			{
				nodeCount = nodeCount + 1;
				temp = temp->next;
			}

			Node<T>* temp2 = first;

			for (int i = 1; i < (nodeCount - 1); i++)
			{
				temp2 = temp2->next;
			}

			cout << temp2->val << endl;
			delete(temp2->next);

			last = temp2;
			last->next = NULL;

			return true;
		}
	}

	bool isEmpty()
	{
		if (first == NULL && last == NULL) { return true; }
		else { return false; }
	}

	int size()
	{
		if (first == NULL && last == NULL) { return 0; }

		Node<T>* temp = first;
		int nodeCounter = 0;

		while (temp != NULL)
		{
			nodeCounter = nodeCounter + 1;
			temp = temp->next;
		}
		return nodeCounter;
	}

	void clear()
	{
		Node<T>* temp = first;
		while (temp != NULL)
		{
			temp = temp->next;
			first = temp;
			delete(temp);
		}
	}

	void insertAtFront(T valueToInsert)
	{
		Node<T>* newNode;

		newNode->val = valueToInsert;

		if (first == NULL) {
			first = newNode;
		}
		else
		{
			newNode->next = first;
			first = newNode;
		}
	}

	bool removeFromFront()
	{
		if (first == NULL && last == NULL) { return false; }

		else
		{
			Node<T>* temp;

			temp = first;
			first = first->next;

			delete(temp);

			return true;
		}
	}

	T& firstNum()
	{
		return first->val;
	}

private:
	Node<T> *first;
	Node<T> *last;
};
