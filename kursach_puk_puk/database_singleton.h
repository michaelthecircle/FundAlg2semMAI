#ifndef MAIN_CPP_DATABASE_SINGLETON_H
#define MAIN_CPP_DATABASE_SINGLETON_H

#include <iostream>
#include "./comparers/int_comparer.h"
#include "definition_of_delivery.h"
#include "./b_plus_tree/associative_container.h"
#include "./memory.h"
#include "./comparers/stdstring_comparer.h"
#include "./b_plus_tree/bplus_tree.h"
#include "./allocator_names/allocator_type.h"
#include "./memory_allocation_mods/memory_with_fit_allocation.h"
#include "./alloc_new_delete/memory_mihuil.h"
#include "./alloc_list/list_memory.h"
#include "./alloc_border/boundary_tags_allocator.h"
#include "./kursach_puk_puk/logger_singleton.h"
#include "./handler/request_handler_with_command_chain.h"
#include "./command/command_add_pool.h"
#include "./command/command_remove_pool.h"
#include "./command/command_add_scheme.h"
#include "./command/command_remove_scheme.h"
#include "./command/command_add_collection.h"
#include "./command/command_remove_collection.h"
#include "./command/command_add_data.h"
#include "./command/command_remove_data.h"
#include "./command/command_get_data.h"
#include "./command/command_get_data_between.h"
#include "./command/command_update.h"
#include "./allocator_names/allocator_type.h"
#include "./comparers/stdstring_comparer.h"

class database_singleton
{
private:
    //TODO тут был компаратор пар
private:
    static database_singleton *_instance;
    request_handler_with_command_chain _chain;
public:
    static database_singleton *get_instance()
    {
        if (_instance == nullptr)
        {
            _instance = new database_singleton();
        }

        return _instance;
    }
private:
    associative_container<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_member *> *> *> *, memory *> > *_database_entrypoint;
private:
    database_singleton()
    {
        _database_entrypoint = new bplus_tree<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_member *> *> *> *, memory *>, stdstring_comparer>(10);

        _chain
                .add_handler(new command_add_pool())
                .add_handler(new command_remove_pool())
                .add_handler(new command_add_scheme())
                .add_handler(new command_remove_scheme())
                .add_handler(new command_add_collection())
                .add_handler(new command_remove_collection())
                .add_handler(new command_add_data())
                .add_handler(new command_remove_data())
                .add_handler(new command_get_data())
                .add_handler(new command_get_data_between())
                .add_handler(new command_update_data());

        _instance = this;
    }
public:
    ~database_singleton()
    {
        delete _database_entrypoint;
    }
    database_singleton(database_singleton const &) = delete;
    database_singleton(database_singleton &&) = delete;
    database_singleton &operator=(database_singleton const &) = delete;
    database_singleton &operator=(database_singleton &&) = delete;
public:
    void add_pool(
            std::string const &pool_name,
            allocator_type pool_allocator_type,
            unsigned int pool_allocator_size,
            allocator_type pool_allocator_allocation_mode)
    {
        try
        {
            memory *allocator = nullptr;

            switch (pool_allocator_allocation_mode) {
                case allocator_type::global_heap:
                    allocator = new memory_mihuil();
                    break;
                case allocator_type::sorted_list:
                    break;
                case allocator_type::boundary_tags:
                    break;
                case allocator_type::buddy_system:
                    break;
            }

            _database_entrypoint->insert(pool_name, std::move(std::make_pair(new bplus_tree<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_member *> *> *, stdstring_comparer>(10), allocator)));

            std::cout << "[DATA BASE] pool added to data base" << std::endl << std::endl;
        }
        catch (std::exception const &ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }
    void add_scheme(std::string const & pool_name, std::string const & scheme_name)
    {
        try
        {
            //const_cast<pool &>(_data_base->find(pool_name)).add(scheme_name, std::move(scheme()));
            const_cast<pool &>(_data_base->find(pool_name)).add(scheme_name, std::move(scheme()));
            std::cout << "[DATA BASE] scheme added to " << pool_name << std::endl << std::endl;
        }
        catch (std::exception const &ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }
};

#endif //MAIN_CPP_DATABASE_SINGLETON_H
