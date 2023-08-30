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

#include "avl_tree/avl_tree.h"

class database_singleton
{
private:
    class stdpair_int_int_comparer final
    {
    private:
        int_comparer _int_comparer;
    public:
        int operator()(
                std::pair<int, int> const &left,
                std::pair<int, int> const &right)
        {
            auto user_id_comparison_result = _int_comparer(left.first, right.first);
            if (user_id_comparison_result != 0)
            {
                return user_id_comparison_result;
            }

            return _int_comparer(left.second, right.second);
        }
    };
private:
    static database_singleton *_instance;
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
    associative_container<std::string, associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_member *> *> *> * > *_database_entrypoint;
    memory* memory;
    logger* logger;
private:
    database_singleton()
    {
        _database_entrypoint = new avl_tree<std::string, associative_container<std::string, associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_member *> *> *> * >, stdstring_comparer>;
        _database_entrypoint = new bplus_tree<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_member *> *> *> *, memory* >,  stdstring_comparer>(10)>;


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
};

#endif //MAIN_CPP_DATABASE_SINGLETON_H
