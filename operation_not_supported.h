#ifndef MAIN_CPP_OPERATION_NOT_SUPPORTED_H
#define MAIN_CPP_OPERATION_NOT_SUPPORTED_H

#include <iostream>

class operation_not_supported final : public std::exception
{
private:
    std::string _what;
public:
    operation_not_supported()
            : _what("operation_not_supported")
    {

    }

    char const* what() const noexcept override
    {
        return _what.c_str();
    }
};

#endif //MAIN_CPP_OPERATION_NOT_SUPPORTED_H
