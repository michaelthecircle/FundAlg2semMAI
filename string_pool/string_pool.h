#ifndef MAIN_CPP_STRING_POOL_H
#define MAIN_CPP_STRING_POOL_H

#include <iostream>
#include "comparers/stdstring_comparer.h"
#include "./b_plus_tree/associative_container.h"
#include "./b_plus_tree/bplus_tree.h"

class string_pool
{
private:
    static string_pool *_instance;
public:
    static string_pool *get_instance()
    {
        if (_instance == nullptr)
        {
            _instance = new string_pool;
        }

        return _instance;
    }

private:

    associative_container<std::string, std::pair<std::string *, unsigned int> > *_pool;

private:

    string_pool()
    {
        _pool = new bplus_tree<std::string, std::pair<std::string *, unsigned int>, stdstring_comparer>(10);
    }

public:

    string_pool(string_pool const &) = delete;

    string_pool(string_pool &&) = delete;

    string_pool &operator=(string_pool const &) = delete;

    string_pool &operator=(string_pool &&) = delete;

    ~string_pool() noexcept
    {
        delete _pool;
    }

public:

    std::string *get_string(std::string const &key)
    {
        //try
        //{
        //    return std::string_pool->get(key);
        //}
        //catch (search_tree<std::string, std::string *, stdstring_comparer>::reading_exception const &ex)
        //{
        //    auto * string_to_insert = new std::string(key);
        //    _pool->insert(key, std::move(string_to_insert));
        //    return _pool->get(key);
        //}
        // TODO: гавнище джяляб пиздэц
        return new std::string("");
    }

    void remove_string(std::string const &key)
    {
        try
        {
            // TODO
        }
        catch (search_tree<std::string, std::string *, stdstring_comparer>::removing_exception const &ex)
        {
            // TODO
        }
    }

};


#endif //MAIN_CPP_STRING_POOL_H
