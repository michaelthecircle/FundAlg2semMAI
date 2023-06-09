#ifndef MAIN_CPP_DATABASE_SINGLETON_H
#define MAIN_CPP_DATABASE_SINGLETON_H
#include "./alloc_list/list_memory.h"
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
#include "./kursach_puk_puk/logger_singleton.h"
#include "./operation_not_supported.h"

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
    associative_container<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_member *> *> *> *, memory *> > *_database_entrypoint;
private:
    database_singleton()
    {
        _database_entrypoint = new bplus_tree<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_member *> *> *> *, memory *>, stdstring_comparer>(10);
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
            memory_with_fit_allocation::allocation_mode pool_allocator_allocation_mode)
    {
        try
        {
            memory *allocator = nullptr;
            switch (pool_allocator_type)
            {
                case allocator_type::sorted_list:
                    allocator = new list_memory(5000, memory::allocation_mode::first_fit,nullptr, nullptr);
                    break;
                case allocator_type::global_heap:
                    break;
                case allocator_type::boundary_tags:
                    break;
                case allocator_type::buddy_system:
                    break;
            }
            _database_entrypoint->insert(pool_name, std::move(std::make_pair(new bplus_tree<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_member *> *> *, stdstring_comparer>(10), allocator)));
        }
        catch (search_tree<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_member *> *> *> *, memory *>, stdstring_comparer>::insertion_exception const &ex)
        {
            throw operation_not_supported();
            return ;
        }
    }
    void add_scheme(
            std::string const &pool_name,
            std::string const &scheme_name
            )
    {
        try
        {
            std::get<0>(_database_entrypoint->get(pool_name))->insert(scheme_name, std::move(bplus_tree<std::pair<int, int>, delivery_member *, stdpair_int_int_comparer>(10));
        }
        catch (search_tree<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_member *> *> *> *, memory *>, stdstring_comparer>::insertion_exception const &ex)
        {
            throw operation_not_supported();
            return ;
        }
    }
};

#endif //MAIN_CPP_DATABASE_SINGLETON_H
