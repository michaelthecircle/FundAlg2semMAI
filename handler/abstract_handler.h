#ifndef MAIN_CPP_ABSTRACT_HANDLER_H
#define MAIN_CPP_ABSTRACT_HANDLER_H

#include <iostream>

class abstract_handler
{

    friend class request_handler_with_command_chain;

protected:

    abstract_handler * _next_handler;

public:

    abstract_handler() : _next_handler(nullptr) {}

    virtual ~abstract_handler() = default;

public:

    virtual bool handle(std::string const & request) const noexcept = 0;
};

#endif //MAIN_CPP_ABSTRACT_HANDLER_H
