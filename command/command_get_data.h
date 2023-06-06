#ifndef MAIN_CPP_COMMAND_REMOVE_DATA_H
#define MAIN_CPP_COMMAND_REMOVE_DATA_H

#include <iostream>
#include "command.h"
#include "./kursach_puk_puk/logger_singleton.h"
#include "./Parse/parse.h"



class command_get_data final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
public:
    bool can_execute(std::string const &request) noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_get_data::can_execute(std::string const &request) called", logger::severity::trace);
        std::vector<std::string> result_string = validation("GET_DATA", request, 3);
        if(!result_string.empty())
        {
            _pool_name = result_string[0];
            _scheme_name = result_string[1];
            _collection_name = result_string[2];
            return true;
        }
        return false;
    }

    void execute(std::string const &request) noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_get_data::execute(std::string const &request) called", logger::severity::trace);
    }
};


#endif //MAIN_CPP_COMMAND_REMOVE_DATA_H
