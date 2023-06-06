#ifndef abobus
#define abobus
#include <iostream>

class not_implemented final : public std::exception
{
private:
    std::string _what;
public:
    not_implemented(std::string const &method_name)
            : _what("Method "+ method_name + " not implemented")
    {

    }
    not_implemented() {}
    char const* what() const noexcept override
    {
        return _what.c_str();
    }
};

#endif