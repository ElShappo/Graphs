#ifndef PRIORITY_QUEUE_EXCEPTION_HPP
#define PRIORITY_QUEUE_EXCEPTION_HPP

template <class T>
class PriorityQueueException : public std::invalid_argument
{
public:
    PriorityQueueException(const char* ex)
    : invalid_argument(ex)
    {

    }
private:
};

#endif // PRIORITY_QUEUE_EXCEPTION_HPP
