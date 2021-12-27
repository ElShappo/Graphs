#ifndef GENERATOR_EXCEPTION_HPP
#define GENERATOR_EXCEPTION_HPP

template <class T>
class GeneratorException : public std::invalid_argument
{
public:
    GeneratorException(const char* ex)
    : invalid_argument(ex)
    {
    }
};

#endif // GENERATOR_EXCEPTION_HPP
