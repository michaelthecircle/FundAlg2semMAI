#ifndef MAIN_CPP_DEFINITION_OF_DELIVERY_H
#define MAIN_CPP_DEFINITION_OF_DELIVERY_H
#include <iostream>


struct delivery_member
{
    std::pair<unsigned int, unsigned int> delivery_pair;
    struct value
    {
        enum class delivery_method
        {
            by_plane,
            by_ship,
            by_car
        };
        std::string surname;
        std::string name;
        std::string patronymic;
        std::string mail;
        std::string phone;
        std::string user_comment;
        std::string date;
    };
};
#endif //MAIN_CPP_DEFINITION_OF_DELIVERY_H
