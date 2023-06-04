#ifndef MAIN_CPP_COMMAND_REMOVE_POOL_H
#define MAIN_CPP_COMMAND_REMOVE_POOL_H

#include <iostream>
#include "command.h"
#include "./kursach_puk_puk/logger_singleton.h"

class command_remove_pool final : public command<std::string>
{
private:
    std::string _pool_name;
public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_remove_pool::can_execute(std::string const &request) called", logger::severity::trace);
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_remove_pool::execute(std::string const &request) called", logger::severity::trace);
    }
};


#endif //MAIN_CPP_COMMAND_REMOVE_POOL_H
