#ifndef MAIN_CPP_POOL_H
#define MAIN_CPP_POOL_H

#include "scheme.h"
#include "../avl_tree/avl_tree.h"
#include "../database/keycomparer.h"
//потом убрать 2 верхние строки
#include <iostream>

class pool final
{

private:


    associative_container<std::string, scheme> * _pool;

public:

    pool() : _pool(new avl_tree<std::string, scheme, key_comparer>()) {};

    ~pool();

public:

    void add(std::string const & key, scheme && target)
    {
        _pool->insert(key, std::move(target));
    }

    void remove(std::string const & key);

    scheme const & find(std::string const & scheme_name) const;

    bool find_in(std::string const & scheme_name) const;


public:

    pool(pool const & other);

    pool & operator=(pool const & other);

    pool(pool && other) noexcept;

    pool & operator=(pool && other) noexcept;

};
#endif //MAIN_CPP_POOL_H
