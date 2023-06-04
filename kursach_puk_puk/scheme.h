#ifndef MAIN_CPP_SCHEME_H
#define MAIN_CPP_SCHEME_H
#include <iostream>
#include "./b_plus_tree/associative_container.h"
#include "./b_plus_tree/bplus_tree.h"
#include "./logger/logger.h"
#include "./memory.h"
#include "trees_names.h"
#include "collection.h"
#include "comparers/stdstring_comparer.h"

class scheme_collection
{
private:
    associative_container<std::string, collection > *_schemes;
public:
    scheme_collection(std::string const &name_of_scheme, trees tree, unsigned int t = 0, memory* mem = nullptr, logger* log_associative = nullptr)
    {
        switch (tree)
        {
            case trees::bplus_tree:
                _schemes = new bplus_tree<std::string, collection, stdstring_comparer>(t, mem, log_associative);
                break;
            default:
                break;
        }
    }
};
#endif //MAIN_CPP_SCHEME_H
