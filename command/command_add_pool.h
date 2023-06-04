#ifndef MAIN_CPP_COMMAND_ADD_POOL_H
#define MAIN_CPP_COMMAND_ADD_POOL_H

#include <iostream>
#include "command.h"
#include "./memory_allocation_mods/memory_with_fit_allocation.h"
#include "./allocator_names/allocator_type.h"
#include "./kursach_puk_puk/logger_singleton.h"
#include "./kursach_puk_puk/database_singleton.h"


class command_add_pool final : public command<std::string>
{
private:
    std::string _pool_name;
    allocator_type _pool_allocator_type;
    unsigned int _pool_allocator_size;
    memory_with_fit_allocation::allocation_mode _pool_allocator_allocation_mode;

public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_pool::can_execute(std::string const &request) called", logger::severity::trace);

        // TODO

        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_pool::execute(std::string const &request) called", logger::severity::trace);
        database_singleton::get_instance()->add_pool(_pool_name, _pool_allocator_type, _pool_allocator_size, _pool_allocator_allocation_mode);
    }
};


#endif //MAIN_CPP_COMMAND_ADD_POOL_H
