#ifndef MAIN_CPP_COMMAND_ADD_POOL_H
#define MAIN_CPP_COMMAND_ADD_POOL_H

#include <iostream>
#include "command.h"
#include "./memory_allocation_mods/memory_with_fit_allocation.h"
#include "./allocator_names/allocator_type.h"
#include "./kursach_puk_puk/logger_singleton.h"
#include "./kursach_puk_puk/database_singleton.h"
#include "Parse/parse.h"

class command_add_pool final : public command<std::string>
{
private:
    std::string _pool_name;
    allocator_type _pool_allocator_type;
    unsigned int _pool_allocator_size;
    memory::allocate_mode _pool_allocator_allocation_mode;
public:
    bool can_execute(std::string const &request) noexcept
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_pool::can_execute(std::string const &request) called", logger::severity::trace);
        if(request.starts_with("ADD_POOL(") && request.ends_with(")"))
        {
            std::string* non_const_request = const_cast<std::string*>(&request);

            std::vector<std::string> chopped_string = chop_string_to_vector(
                    non_const_request->begin() + std::string("ADD_POOL").length(),
                    non_const_request->end(),
                    ' ');
            if(chopped_string.size() != 3) return false;
            _pool_name = std::move(chopped_string[0]);
            // TODO _pool_allocator_allocation_mode = chopped_string[1];
            //TODO ? _collection_name = chopped_string[2];
            return true;
        }
        return false;
    }

    void execute(std::string const &request) noexcept
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_pool::execute(std::string const &request) called", logger::severity::trace);
        //TODO database_singleton::get_instance()->add_pool(_pool_name, _pool_allocator_type, _pool_allocator_size, _pool_allocator_allocation_mode);
    }
};


#endif //MAIN_CPP_COMMAND_ADD_POOL_H
