#include <iostream>

class not_implemented final : public std::exception
{
public:
    char const* what()const noexcept override
    {
        return "Method not implemented";
    }
};