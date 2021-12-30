#ifndef SEQUENCE_EXCEPTION_HPP
#define SEQUENCE_EXCEPTION_HPP

#include <exception>
#include <iostream>

template <class T>
class SequenceException : public std::invalid_argument
{
public:
    SequenceException(const char* ex)
    : invalid_argument(ex)
    {

    }
private:
};

#endif // SEQUENCE_EXCEPTION_HPP
