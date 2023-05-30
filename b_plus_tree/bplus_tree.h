#ifndef SANDBOX_CPP_BPLUS_TREE_H
#define SANDBOX_CPP_BPLUS_TREE_H
#include <stack>
#include "associative_container.h"
#include "./logger/logger.h"
#include "./logger/logger_holder.h"
#include "./memory.h"
#include "search_tree.h"
#include "./not_implemented.h"
#include "./alloc_new_delete/memory_holder.h"

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
class bplus_tree final:
    public search_tree<tkey, tvalue, tkey_comparer>
{

private:

    unsigned int _t;

public:

    explicit bplus_tree(
        unsigned int t,
        memory *allocator = nullptr,
        logger *logger = nullptr)
        : search_tree<tkey, tvalue, tkey_comparer>(allocator, logger, new typename search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context(this)),
          _t(t)
    {

    }

public:

    bplus_tree(bplus_tree<tkey, tvalue, tkey_comparer> const &other)
    {
        // TODO
    }

    bplus_tree<tkey, tvalue, tkey_comparer> &operator=(bplus_tree<tkey, tvalue, tkey_comparer> const &other)
    {
        if (this == &other)
        {
            return *this;
        }

        // TODO

        return *this;
    }

    bplus_tree(bplus_tree<tkey, tvalue, tkey_comparer> &&other) noexcept
    {
        // TODO
    }

    bplus_tree<tkey, tvalue, tkey_comparer> &operator=(bplus_tree<tkey, tvalue, tkey_comparer> &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        // TODO

        return *this;
    }

    ~bplus_tree() = default;

private:

    std::tuple<std::stack<typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *>,
               typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node **,
               unsigned int,
               bool> find_path(tkey const &key)
    {
        std::stack<typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *> path;
        // TODO: чё за хуйня -_- (search_tree<tkey, tvalue, tkey_comparer>::_root)
        typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node **iterator = &(search_tree<tkey, tvalue, tkey_comparer>::_root);
        unsigned int target_key_index = 0;
        tkey_comparer comparer;

        while (*iterator != nullptr)
        {
            // TODO: dihotomy search

            for (auto i = 0; i < (*iterator)->involved_keys; i++)
            {
                auto comparison_result = comparer(key, (*iterator)->keys_and_values[i].key);
                if (comparison_result == 0)
                {
                    if ((*iterator)->subtrees[0] == nullptr)
                    {
                        return { path, iterator, i, true };
                    }

                    path.push(*iterator);
                    iterator = (*iterator)->subtrees + (*iterator)->involved_keys;
                    break;
                }
                else if (comparison_result > 0)
                {
                    path.push(*iterator);
                    iterator = (*iterator)->subtrees + i;
                    if (*iterator == nullptr)
                    {
                        return { path, iterator, i, false };
                    }
                    break;
                }
            }
        }

        return { path, iterator, target_key_index, false };
    }

public:

    void insert(
        tkey const &key,
        tvalue &&value) override
    {
        auto path_info = find_path(key);
        if (std::get<3>(path_info))
        {
            throw typename search_tree<tkey, tvalue, tkey_comparer>::insertion_exception();
        }

        auto **target_ptr = std::get<1>(path_info);

        if (std::get<0>(path_info).size() == 0 && *target_ptr == nullptr)
        {
            *target_ptr = search_tree<tkey, tvalue, tkey_comparer>::_context->create_node(get_max_keys_count());
            (*target_ptr)->involved_keys++;
            (*target_ptr)->keys_and_values[0].key = key;
            (*target_ptr)->keys_and_values[0].value = std::move(value);
        }
        else if ((*target_ptr)->involved_keys < get_max_keys_count())
        {

        }
    }

    tvalue const &get(
        tkey const &key) override
    {
        throw std::exception();
    }

    tvalue remove(
        tkey const &key) override
    {
        throw std::exception();
    }

private:

    inline unsigned int get_min_keys_count() const noexcept
    {
        return _t - 1;
    }

    inline unsigned int get_max_keys_count() const noexcept
    {
        return get_min_keys_count() * 2;
    }

};

#endif //SANDBOX_CPP_BPLUS_TREE_H
