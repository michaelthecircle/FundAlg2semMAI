#ifndef sobaka
#define sobaka
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

    virtual tvalue const &get(
        tkey const &key) = 0;

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
    return get(key);
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