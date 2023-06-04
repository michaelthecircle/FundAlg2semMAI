#ifndef MAIN_CPP_INT_COMPARER_H
#define MAIN_CPP_INT_COMPARER_H


class int_comparer final
{

public:

    int operator()(
            int left,
            int right)
    {
        return left - right;
    }

};

#endif //MAIN_CPP_INT_COMPARER_H
