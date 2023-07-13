#ifndef FUNDAMENTAL_ALGO_COMMAND_ADD_COLLECTION_H
#define FUNDAMENTAL_ALGO_COMMAND_ADD_COLLECTION_H
#include <iostream>
#include "./kursach_puk_puk/logger_singleton.h"
#include "command.h"
#include <vector>
#include "./Parse/parse.h"

class command_add_collection final : public command
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
public:

    bool can_execute(std::string const &request) noexcept final
    {

        logger_singleton::get_instance()->get_logger()->
        log("command_add_collection::can_execute(std::string const &request) called", logger::severity::trace);
        if(!prefix_validation("ADD_COLLECTION", request)) return false;
        std::vector<std::string> result_parsed_strings = validation("ADD_COLLECTION", request, 3);
        if(!result_parsed_strings.empty())
        {
            _pool_name = std::move(result_parsed_strings[0]);
            _scheme_name = std::move(result_parsed_strings[1]);
            _collection_name = std::move(result_parsed_strings[2]);
            return true;
        }
        return false;

    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_collection::execute(std::string const &request) called", logger::severity::trace);
    }
};
#endif //FUNDAMENTAL_ALGO_COMMAND_ADD_COLLECTION_H
