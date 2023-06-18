//
// Created by Admin on 07.06.2023.
//

#ifndef MAIN_CPP_STD_PAIR_COMPARER_H
#define MAIN_CPP_STD_PAIR_COMPARER_H
#include "int_comparer.h"
#include <iostream>

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

#endif //MAIN_CPP_STD_PAIR_COMPARER_H
