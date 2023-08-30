#ifndef MAIN_CPP_KEYCOMPARER_H
#define MAIN_CPP_KEYCOMPARER_H

#include "./kursach_puk_puk/definition_of_delivery.h"

class key_comparer final
{

public:

    int operator()(delivery_member::key * first, delivery_member::key * second)
    {
        int comparison_result = first->delivery_id - second->delivery_id;
        if (comparison_result != 0)
        {
            return comparison_result;
        }

        comparison_result = first->user_id - second->user_id;
        if (comparison_result != 0)
        {
            return comparison_result;
        }
    }

};
#endif //MAIN_CPP_KEYCOMPARER_H
