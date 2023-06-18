#ifndef MAIN_CPP_DATA_COLLECTION_H
#define MAIN_CPP_DATA_COLLECTION_H

#include "./alloc_new_delete/memory_holder.h"
#include "./memory.h"
#include "./b_plus_tree/bplus_tree.h"
#include "./kursach_puk_puk/definition_of_delivery.h"

class data_collection final : protected memory_holder
{

private:
//
    memory * _allocator;

    associative_container<key*, value*> * _data;

public:

    data_collection(memory * allocator = nullptr);

    ~data_collection();

public:

    void add(
            unsigned int id_session,
            unsigned int id_student,
            form format,
            const std::string &surname,
            const std::string &name,
            const std::string &patronymic,
            const std::string &mail,
            const std::string &phone,
            const std::string &user_comment,
            const std::string &date);

    value remove(key * data_key);

    value * get(key * const & data_key);

    std::vector<value *> get_between_keys(key * const & min_key, key * const & max_key);

    void update(
            unsigned int id_session,
            unsigned int id_student,
            form format,
            const std::string & subject,
            const std::string & surname,
            const std::string & name,
            const std::string & patronymic,
            const std::string & date,
            const std::string & time,
            unsigned int mark);

public:

    data_collection(data_collection const & other);

    data_collection & operator=(data_collection const & other);

    data_collection(data_collection && other) noexcept;

    data_collection & operator=(data_collection && other) noexcept;

private:

    memory * get_outer_allocator() const override;

};

#endif //MAIN_CPP_DATA_COLLECTION_H
