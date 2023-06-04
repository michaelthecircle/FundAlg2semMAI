#ifndef MAIN_CPP_DATABASE_H
#define MAIN_CPP_DATABASE_H
#include <iostream>
#include "./b_plus_tree/associative_container.h"
#include "./b_plus_tree/bplus_tree.h"
#include "./logger/logger.h"
#include "./memory.h"
#include "scheme.h"
#include "trees_names.h"
#include "comparers/stdstring_comparer.h"

class pool_collection
{
private:
    associative_container<std::string, std::pair<scheme_collection, memory *> > *_pools;
public:
    pool_collection(std::string const &name_of_pool, trees tree, unsigned int t = 0, memory* mem = nullptr, logger* log_associative = nullptr)
    {
        switch (tree)
        {
            case trees::bplus_tree:
                _pools = new bplus_tree<std::string, std::pair<scheme_collection, memory *>, stdstring_comparer>(t, mem, log_associative);
                break;
            default:
                break;
        }
    }
};
#endif //MAIN_CPP_DATABASE_H
