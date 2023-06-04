#ifndef MAIN_CPP_REQUEST_HANDLER_WITH_COMMAND_CHAIN_H
#define MAIN_CPP_REQUEST_HANDLER_WITH_COMMAND_CHAIN_H
#include "abstract_handler.h"
#include "request_handler_with_command.h"

template<typename trequest> class request_handler_with_command_chain final
{
private:
    abstract_handler<trequest> *_first_handler;
    abstract_handler<trequest> *_last_handler;
public:
    request_handler_with_command_chain()
            : _first_handler(nullptr), _last_handler(nullptr)
    {

    }
    ~request_handler_with_command_chain()
    {
        while (_first_handler != nullptr)
        {
            _last_handler = _first_handler;
            _first_handler = _first_handler->_next_handler;
            delete _last_handler;
        }
    }
    request_handler_with_command_chain(request_handler_with_command_chain const &) = delete;
    request_handler_with_command_chain(request_handler_with_command_chain &&) = delete;
    request_handler_with_command_chain &operator=(request_handler_with_command_chain const &) = delete;
    request_handler_with_command_chain &operator=(request_handler_with_command_chain &&) = delete;
public:
    bool handle(trequest const &request) const noexcept
    {
        if (_first_handler == nullptr)
        {
            return false;
        }

        return _first_handler->handle(request);
    }
public:
    request_handler_with_command_chain &add_handler(command<trequest> *cmd)
    {
        _last_handler = _first_handler == nullptr
                        ? _first_handler = new request_handler_with_command<trequest>(cmd)
                        : _last_handler->_next_handler = new request_handler_with_command<trequest>(cmd);

        return *this;
    }
};


#endif //MAIN_CPP_REQUEST_HANDLER_WITH_COMMAND_CHAIN_H
