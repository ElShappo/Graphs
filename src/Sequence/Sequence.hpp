#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <iostream>
#include <vector> // vector and string are used only once when plotting Histogram (github library sciplot works only with vectors)
#include <type_traits>
#include "SequenceException.hpp"
#include "epsilon.h"


namespace My
{
    template <class T>
    class Sequence
    {
    public:

        // --------------- CTORS

        Sequence() {}

        Sequence(T* items, int size)
        {
            if (size < 0 || (size > 0 && items == NULL))
                throw SequenceException<T>("OutOfRange");

            reserve(size);
            size_ = capacity_; // in this case length equals to size

            for (int i=0; i<size_; ++i)
                this->arr_[i] = items[i];
        }
        Sequence(const Sequence<T> & Sequence)
        {
            reserve(Sequence.capacity());
            size_ = Sequence.size();

            arr_ = new T[size_];

            for (int i=0; i<size_; ++i)
                arr_[i] = Sequence.get(i);
        }

        T & operator[](const int index) const
        {
            if (index >= size_)
                throw SequenceException<T>("IndexOutOfRange");

            return arr_[index];
        }

        bool operator ==(const Sequence<T> & other) const
        {
            if (other.size() != size() )
                return false;

            for (int i=0; i<other.size(); ++i)
            {
                if constexpr (std::is_floating_point_v<T>)
                {
                    if (!areSame(other.get(i), get(i)))
                    {
                        return false;
                    }
                }
                else
                {
                    if (other.get(i) != get(i))
                        return false;
                }
            }
            return true;
        }

        bool operator !=(const Sequence<T> & other) const
        {
            if (*this == other)
                return false;
            return true;
        }

        Sequence<T> & operator =(const Sequence<T> & other)
        {
            while (!empty() )
                pop_back();

            for (int i=0; i<other.size(); ++i)
                push_back(other[i]);
            return *this;
        }

        ~Sequence()
        {
            delete[] arr_;
        }

        // ----------------------


        T get(int index) const
        {
            if (index >= size_ || index < 0)
            {
                std::cout << index << " > " << size_ << std::endl;
                throw SequenceException<T>("IndexOutOfRange");
            }
            return arr_[index];
        }

        int capacity() const noexcept
        {
            return capacity_;
        }

        int size() const noexcept
        {
            return size_;
        }

        void set(T value, int index)
        {
            if (index > size_-1 || index < 0)
            {
                std::cout << index << " > " << size_ << std::endl;
                throw SequenceException<T>("IndexOutOfRange");
            }
            arr_[index] = value;
        }

        std::vector<T> & to_vector() const
        {
            std::vector<T>* vec = new std::vector<T>();

            for (int i=0; i<size(); ++i)
                vec->push_back(get(i));

            return *vec;
        }

        static Sequence<T> & toSequence(std::vector<T> vec)
        {
            Sequence<T>* arr = new Sequence<T>();

            for (auto it : vec)
                arr->push_back(it);

            return *arr;
        }

        void reserve(int capacity)
        // when the object is uninitialized, whole new chunk of memory is allocated
        // otherwise already existing chunk is changed
        {
            if (capacity < 0)
                throw SequenceException<T>("OutOfRange");

            if (arr_ == NULL)
                arr_ = (T*)realloc(NULL, sizeof(T)*capacity);
            else
                arr_ = (T*)realloc(arr_, sizeof(T)*capacity);

            capacity_ = capacity;

            if (capacity_ < size_)
                size_ = capacity_;
        }

        /*
        void resize(int len)
        // pretty much the same as pop_back
        // this method does not allow to increase the length
        {
            if (size_ < len || len < 0)
                throw SequenceException<T>("IndexOutOfRange");
            else
                size_ = len;

            for (int i=0; i<size_-len; ++i)
                pop_back();
        }

        void Swap(int pos1, int pos2)
        {
            if (pos1 < 0 || pos2 < 0)
                throw SequenceException<T>("OutOfRange");

            swap(arr_[pos1], arr_[pos2]);
        }
        */

        void insert(T data, int pos)
        {
            if (pos > size_ || pos < 0)
            {
                std::cout << pos << " > " << size_ << std::endl;
                throw SequenceException<T>("IndexOutOfRange");
            }

            if (arr_ == NULL)
                arr_ = (T*)realloc(NULL, sizeof(T)*(size_+1));
            else
                arr_ = (T*)realloc(arr_, sizeof(T)*(size_+1));

            ++size_;

            for (int i=size_-2; i>=pos; --i)
                set(get(i), i+1);

            arr_[pos] = data;

            if (capacity_ < size_)
                capacity_ = size_;
        }

        void push_back(T data)
        {
            insert(data, size_);
        }
        void push_front(T data)
        {
            insert(data, 0);
        }
        void append(Sequence<T> buffer)
        {
            for (int i=0; i<buffer.GetLen(); ++i)
                push_back(buffer.get(i));
        }

        T pop_back()
        {
            if (arr_ == NULL || size_ == 0 || capacity_ == 0)
                throw SequenceException<T>("Can't pop empty array");
            else
            {
                T buffer = arr_[size_-1];
                arr_ = (T*)realloc(arr_, sizeof(T)*(size_-1));
                --size_;
                return buffer;
            }
        }

        int getIndex(const T element) const
        // check if such element exists, then return it's index
        // otherwise assign -1 to the index
        {
            for (int i=0; i<this->size(); ++i)
            {
                if (get(i) == element)
                    return i;
            }
            return -1;
        }

        void remove(const int index)
        // remove element with the specified index
        {
            if (index < 0 || index >= this->size())
                throw SequenceException<T>("index out of range");

            int newSize = this->size()-1;
            //cout << "NewSize: " << newSize << endl;
            T* newArr = new T[newSize];

            int j = 0;
            for (int i=0; i<this->size(); ++i)
            {
                if (i != index)
                    newArr[j] = get(i);
                else
                    continue; // skip this i
                ++j;
            }

            delete[] arr_;

            arr_ = newArr;
            size_ = newSize;
        }

        void removeAllOccurrences(const T element)
        // remove all occurrences of element
        {
            while (getIndex(element) != -1)
            {
                int index = getIndex(element);
                remove(index);
            }
        }

        void removeRepetitions(const T element)
        // leave only 1 occurrence of the element
        {
            int index = getIndex(element);
            int prevIndex = index;

            while (index != -1) // while there are such elements
            {
                index = getIndex(element);
                //cout << "[" << index << "]: " << element << endl;

                if (index != -1)
                    prevIndex = index;
                else
                    break;
                remove(index);
            }
            //cout << "removeRep" << endl;
            insert(element, prevIndex);
        }

        void shrink_to_fit(int delta = 1)
        {
            if (abs((int)capacity_-(int)size_) > delta)
                capacity_ = size_;
        }

        bool empty() const noexcept
        {
            if (size_ == 0)
                return true;
            return false;
        }

        void erase()
        {
            size = 0;
            arr_ = nullptr;
        }

        T getMax() const
        {
            T max = 0;

            for (int i=0; i<this->size(); ++i)
            {
                if constexpr (std::is_arithmetic_v<T>)
                {
                    if (max < get(i))
                        max = get(i);
                }
                else
                    throw SequenceException("unsupported datatype for Dictionary keys has been used");
            }
            return max;
        }

        void print()
        {
            for (int i=0; i<this->size(); ++i)
                std::cout << get(i) << std::endl;
        }

    private:

        T* arr_ = NULL;
        int capacity_ = 0;
        int size_ = 0;
    };

}

#endif // SEQUENCE_HPP

