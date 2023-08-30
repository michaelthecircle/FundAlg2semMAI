#ifndef MAIN_CPP_DATA_COLLECTION_H
#define MAIN_CPP_DATA_COLLECTION_H
#include "./alloc_new_delete/memory_holder.h"
#include "./memory.h"
#include "./b_plus_tree/bplus_tree.h"
#include "./kursach_puk_puk/definition_of_delivery.h"
#include "avl.h"
#include "bst.h"
#include "./database/keycomparer.h"

class data_collection final : protected memory_holder
{

private:
    memory * _allocator;
    associative_container<delivery_member::key*, delivery_member::value*> * _data;

public:

    data_collection(memory * allocator = nullptr) : _data(new binary_search_tree<std::string , int, key_comparer>(
            nullptr, nullptr)) {};

    ~data_collection();

public:

    void add(
            unsigned int user_id,
            unsigned int delivery_id,
            delivery_member::value::delivery_method method,
            const std::string &surname,
            const std::string &name,
            const std::string &patronymic,
            const std::string &mail,
            const std::string &phone,
            const std::string &user_comment,
            const std::string &date);

    delivery_member::value remove(delivery_member::key * data_key);

    delivery_member::value * get(delivery_member::key * const & data_key);

    std::vector<delivery_member::value *> get_between_keys(delivery_member::key * const & min_key, delivery_member::key * const & max_key);

    void update(
            unsigned int user_id,
            unsigned int delivery_id,
            delivery_member::value::delivery_method method,
            const std::string &surname,
            const std::string &name,
            const std::string &patronymic,
            const std::string &mail,
            const std::string &phone,
            const std::string &user_comment,
            const std::string &date);

public:

    data_collection(data_collection const & other);

    data_collection & operator=(data_collection const & other);

    data_collection(data_collection && other) noexcept;

    data_collection & operator=(data_collection && other) noexcept;


};
#endif //MAIN_CPP_DATA_COLLECTION_H
