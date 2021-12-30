#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include "../Sequence/Sequence.hpp"
#include "PriorityQueueException.hpp"

#include <iostream>

using namespace std;

template <typename P, typename T>
struct QueueHeap
{
    void heapify(int i, int size, My::Sequence<P> & priorities, My::Sequence<T> & values)
    // i is the index of the current node
    // std::pair<int, int> holds the indexes which values had been swapped
    {
        int largest = i; // mark the current element as the largest
        int l = 2 * i + 1; // index of the left child
        int r = 2 * i + 2; // index of the right child

        if (l < size && priorities[l] > priorities[largest]) // check the values held by current element and the left child
            largest = l;

        if (r < size && priorities[r] > priorities[largest]) // check the values held by current element and the right child
            largest = r;

        if (largest != i) // if they do not compare equal
        {
            swap(priorities[i], priorities[largest]); // swap priorities
            swap(values[i], values[largest]); // swap corresponding values
            heapify(largest, size, priorities, values); // recursively call heapify to other indexes of the binary tree
        }
    }

    void insert(My::Sequence<P> & priorities, My::Sequence<T> & values, P newPriority, T newValue)
    {
        int size = priorities.size();
        if (size == 0)
        {
            priorities.push_back(newPriority);
            values.push_back(newValue);
        }
        else
        {
            priorities.push_back(newPriority);
            values.push_back(newValue);

            for (int i = size / 2 - 1; i >= 0; i--)
                heapify(i, size, priorities, values);
      }
    }

    void append(My::Sequence<P> & priorities, My::Sequence<T> & values, My::Sequence<P> & newPriorities, My::Sequence<T> & newValues)
    {
        for (int i=0; i<newValues.size(); ++i)
            insert(priorities, values, newPriorities[i], newValues[i]);
    }

    /*
    void remove(My::Sequence<P> & priorities, My::Sequence<T> & values, P priority)
    // delete Node with the specified priority
    {
        int size = priorities.size();
        int i;
        for (i = 0; i < size; i++)
        {
            if (priority == priorities[i])
                break;
        }
        swap(&priorities[i], &priorities[size - 1]); // swap the last element with the one that is to be deleted
        priorities.pop_back(); // remove the last element
        values.pop_back();

        for (int i = (size-1) / 2 - 1; i >= 0; i--) // recursively heapify
            heapify(i, size, priorities, values);
    }
    */
};

template <typename P, typename T>
class PriorityQueue
{
private:
    My::Sequence<P> priorities_;
    My::Sequence<T> values_;

    void heapSort()
    {
        QueueHeap<P, T> heap;
        int size = values_.size();

        for (int i = size / 2 - 1; i >= 0; i--)
            heap.heapify(i, size, priorities_, values_);

        // Heap heapSort
        for (int i = size - 1; i >= 0; i--) {
            swap(priorities_[0], priorities_[i]);
            swap(values_[0], values_[i]);

            // Heapify root element to get highest element at root again
            heap.heapify(0, i, priorities_, values_);
        }
    }

public:
    PriorityQueue() {}

    PriorityQueue(My::Sequence<P> & priorities, My::Sequence<T> & values)
    {
        QueueHeap<P, T> heap;

        if (priorities.size() != values.size())
            throw PriorityQueueException<T>("number of values and priorities passed do not correspond to one another");
        if (priorities.size() == 0)
            throw PriorityQueueException<T>("cannot pass empty priorities with values");

        priorities_ = priorities;
        values_ = values;

        heapSort();
    }

    PriorityQueue(PriorityQueue<P, T> & other)
    {
        if (other.size() == 0)
            throw PriorityQueueException<T>("cannot pass empty queue");

        priorities_ = other.getPriorities();
        values_ = other.getValues();
    }

    T peek(int index) const {
        if (size() <= index)
            throw PriorityQueueException<T>("index out of range");
        return values_[index];
    }

    T peekFirst() const {
        if (values_.size() == 0)
            throw PriorityQueueException<T>("can't get first element from the priorityQueue: array is empty");
        return values_[0];
    }

    T peekLast() const {
        if (values_.size() == 0)
            throw PriorityQueueException<T>("can't get last element from the priorityQueue: array is empty");
        return values_[values_.size()-1];
    }

    size_t size() const {return values_.size();}

    void add(P newPriority, T newValue)
    {
        QueueHeap<P, T> heap;
        heap.insert(priorities_, values_, newPriority, newValue);
        heapSort();
    }

    void append(My::Sequence<P> newPriorities, My::Sequence<T> newValues)
    {
        QueueHeap<P, T> heap;
        heap.append(priorities_, values_, newPriorities, newValues);
        heapSort();
    }

    T pop_back()
    {
        if (size() == 0)
            throw PriorityQueueException<T>("cannot pop from empty queue");
        T last = peekLast();
        priorities_.pop_back();
        values_.pop_back();
        return last;
    }

    void print()
    {
        cout << "Priorities: ";
        for (int i=0; i<size(); ++i)
        {
            cout << priorities_[i] << " ";
        }
        cout << endl;
        cout << "    Values: ";
        for (int i=0; i<size(); ++i)
        {
            cout << values_[i] << " ";
        }
    }

    T & operator[] (size_t index)
    {
        if (size() <= index)
            throw PriorityQueueException<T>("index out of range");
        return values_[index];
    }

    My::Sequence<P> getPriorities() const
    {
        return priorities_;
    }

    My::Sequence<T> getValues() const
    {
        return values_;
    }

    bool operator== (const PriorityQueue<P, T> & priorityQueue)
    {
        if (priorities_ == priorityQueue.getPriorities())
        {
            if (values_ == priorityQueue.getValues())
                return true;
            return false;
        }
        return false;
    }

    bool operator!= (const PriorityQueue<P, T> & priorityQueue)
    {
        if (*this == priorityQueue)
            return false;
        return true;
    }

    void operator= (PriorityQueue<P, T> & other)
    {
        if (other.size() != 0)
        {
            priorities_ = other.getPriorities();
            values_ = other.getValues();
        }
    }
};

#endif // PRIORITY_QUEUE_HPP
