#ifndef sobaka
#define sobaka

#include "./operation_not_supported.h"
#include <vector>

template<
        typename tkey,
        typename tvalue>
class associative_container
{

public:

    struct key_value_pair final
    {

        tkey key;
        tvalue value;

    };

public:

    virtual ~associative_container() noexcept = default;

public:

    virtual void insert(
            tkey const &key,
            tvalue &&value) = 0;
//TODO: changed method called "get" to "find"
    //virtual tvalue const &get(
    //        tkey const &key) = 0;
    virtual bool find(typename associative_container<tkey, tvalue>::key_value_pair *target_key_and_result_value) = 0;
    virtual std::vector<typename associative_container<tkey, tvalue>::key_value_pair> get_in_range(
            tkey const &lower_bound_inclusive,
            tkey const &upper_bound_inclusive);

    virtual tvalue remove(
            tkey const &key) = 0;

public:

    void operator+=(
            std::pair<tkey const &, tvalue &&> to_insert);

    tvalue const &operator[](
            tkey const &key);

    tvalue operator-=(
            tkey const &key);

};

template<
        typename tkey,
        typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> associative_container<tkey, tvalue>::get_in_range(
        tkey const &lower_bound_inclusive,
        tkey const &upper_bound_inclusive)
{
    throw operation_not_supported();
}

template<
        typename tkey,
        typename tvalue>
void associative_container<tkey, tvalue>::operator+=(
        std::pair<tkey const &, tvalue &&> to_insert)
{
    insert(to_insert.first, std::move(to_insert.second));
}

template<
        typename tkey,
        typename tvalue>
tvalue const &associative_container<tkey, tvalue>::operator[](
        tkey const &key)
{
    //changed "get" to "find"
    return find(key);
}

template<
        typename tkey,
        typename tvalue>
tvalue associative_container<tkey, tvalue>::operator-=(
        tkey const &key)
{
    return remove(key);
}

#endif