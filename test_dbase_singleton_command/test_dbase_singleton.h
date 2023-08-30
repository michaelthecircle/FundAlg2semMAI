#ifndef MAIN_CPP_TEST_DBASE_SINGLETON_H
#define MAIN_CPP_TEST_DBASE_SINGLETON_H

#include "avl.h"
#include "bst.h"
#include "pool.h"
#include "./comparers/stdstring_comparer.h"

class dbase_singleton
{
    dbase_singleton()
    {
        _data_base = new avl_tree<std::string, pool, stdstring_comparer>();
    };
    static dbase_singleton* _dbase_singleton;
    //object of tree
    associative_container<std::string, pool>  * _data_base;

public:
    dbase_singleton(const dbase_singleton&) = delete;
    void operator=(const dbase_singleton&) = delete;
    static dbase_singleton* get_dbase()
    {
        if (_dbase_singleton == nullptr)
        {
            _dbase_singleton = new dbase_singleton();
        }
        return _dbase_singleton;
    }
    //{type of tree} select data - можем вернуть data
    void insertData()
    {

    };

};



#endif //MAIN_CPP_TEST_DBASE_SINGLETON_H
