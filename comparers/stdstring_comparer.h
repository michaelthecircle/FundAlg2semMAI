#ifndef MAIN_CPP_STDSTRING_COMPARER_H
#define MAIN_CPP_STDSTRING_COMPARER_H
#include <iostream>

class stdstring_comparer final
{
public:
    int operator()(
            std::string const& x,
            std::string const &y)
    {
        auto x_iterator = x.begin();
        auto y_iterator = y.begin();
        auto const x_end_iterator = x.cend();
        auto const y_end_iterator = y.cend();

        while (true)
        {
            auto const diff = *x_iterator - *y_iterator;
            if (diff != 0)
            {
                return diff;
            }

            ++x_iterator;
            ++y_iterator;

            if (x_iterator == x_end_iterator && y_iterator == y_end_iterator)
            {
                return 0;
            }
            else if (x_iterator == x_end_iterator)
            {
                return *y_iterator;
            }
            else if (y_iterator == y_end_iterator)
            {
                return *x_iterator;
            }
        }
    }
};
#endif //MAIN_CPP_STDSTRING_COMPARER_H
