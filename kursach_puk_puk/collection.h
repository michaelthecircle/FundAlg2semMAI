#ifndef MAIN_CPP_COLLECTION_H
#define MAIN_CPP_COLLECTION_H
#include <iostream>
#include "./b_plus_tree/associative_container.h"
#include "./b_plus_tree/bplus_tree.h"
#include "./logger/logger.h"
#include "./memory.h"
#include "definition_of_delivery.h"
#include "trees_names.h"
#include "comparers/stdstring_comparer.h"

class collection:
        public delivery_member
{
private:
    associative_container<std::string, associative_container<std::pair<unsigned int, unsigned int>, delivery_member::value>* > *_collections;
public:
    collection(std::string const &name_of_scheme, trees tree, unsigned int t = 0, memory* mem = nullptr, logger* log_associative = nullptr)
    {
        switch (tree)
        {
            case trees::bplus_tree:
                _collections = new bplus_tree<std::string, associative_container<std::pair<unsigned int, unsigned int>, delivery_member::value>*,
                        stdstring_comparer>(t, mem, log_associative);
                break;
            default:
                break;
        }
    }
};
#endif //MAIN_CPP_COLLECTION_H
