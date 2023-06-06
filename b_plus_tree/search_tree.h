#ifndef DATA_STRUCTURES_CPP_SEARCH_TREE_H
#define DATA_STRUCTURES_CPP_SEARCH_TREE_H

#include <exception>
#include <optional>
#include "associative_container.h"
#include "./memory.h"
#include "./alloc_new_delete/memory_holder.h"
#include <iostream>
#include "./logger/logger_holder.h"

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
class search_tree:
    public associative_container<tkey, tvalue>,
    public memory_holder,
    public logger_holder
{

public:

    struct search_tree_node
    {

        typename associative_container<tkey, tvalue>::key_value_pair *keys_and_values;
        search_tree_node **subtrees;
        unsigned int involved_keys;
        unsigned int min_keys_count;

    public:

        virtual ~search_tree_node() noexcept = default;

    };

public:

    class node_interaction_context:
        public memory_holder,
        public logger_holder
    {

    public:

        enum class node_split_mode
        {
            do_not_copy_median_kvp = 0,
            copy_median_kvp = 1
        };

    public:

        search_tree<tkey, tvalue, tkey_comparer> *_target;

    public:

        node_interaction_context(
            search_tree<tkey, tvalue, tkey_comparer> *target)
            : _target(target)
        {

        }

    public:

        typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *create_node_shell(
            unsigned int key_value_pairs_min_count);

        typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *clone_node_with_subtrees(
            typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node_shell);

        void destroy_node(
            typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node_shell,
            bool recursive = false);

        std::optional<std::pair<search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *, typename associative_container<tkey, tvalue>::key_value_pair> > shift(
            typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node,
            unsigned int index_to_vacate,
            typename search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::node_split_mode split_mode,
            tkey const &key,
            tvalue &&value,
            typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *vacation_right_subtree);

    public:

        virtual size_t get_node_shell_size() const noexcept;

        virtual void node_constructor_call(
            search_tree_node *allocated_memory) const noexcept;

        virtual void node_additional_data_injector(
            search_tree_node *initialized_memory) const noexcept;

    public:

        memory *get_memory() const noexcept final;

    public:

        logger *get_logger() const noexcept final;

    };

public:

    enum class insertion_of_existing_key_mode
    {
        throw_an_exception,
        update_value
    };

    enum class removing_of_node_with_two_subtrees_swap_mode
    {
        with_max_from_left_subtree,
        with_min_from_right_subtree
    };

public:

    class insertion_exception final:
        public std::exception
    {

    public:

        char const *what() const noexcept override;

    };

    class reading_exception final:
        public std::exception
    {

    public:

        char const *what() const noexcept override;

    };

    class removing_exception final:
        public std::exception
    {

    public:

        char const *what() const noexcept override;

    };

public:

    search_tree_node *_root;
    logger *_logger;
    memory *_allocator;
    node_interaction_context *_context;

public:

    explicit search_tree(
        memory *allocator = nullptr,
        logger *logger = nullptr)
        : _allocator(allocator), _logger(logger), _context(new node_interaction_context(this))
    {

    }

    explicit search_tree(
        memory *allocator,
        logger *logger,
        node_interaction_context *context);

public:

    search_tree(search_tree<tkey, tvalue, tkey_comparer> const &other);

    search_tree<tkey, tvalue, tkey_comparer> &operator=(search_tree<tkey, tvalue, tkey_comparer> const &other);

    explicit search_tree(search_tree<tkey, tvalue, tkey_comparer> &&other) noexcept;

    search_tree<tkey, tvalue, tkey_comparer> &operator=(search_tree<tkey, tvalue, tkey_comparer> &&other) noexcept;

    ~search_tree();

public:

    memory *get_memory() const noexcept final;

public:

    logger *get_logger() const noexcept final;

};

// region search_tree<tkey, tvalue, tkey_comparer> implementation

// region search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::create_node_shell(
    unsigned int key_value_pairs_min_count)
{
    auto key_value_pairs_max_count = key_value_pairs_min_count << 1;

    auto *node_shell = reinterpret_cast<search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *>(allocate_with_guard(get_node_shell_size()));
    new (node_shell) typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node;

    node_shell->keys_and_values = reinterpret_cast<typename associative_container<tkey, tvalue>::key_value_pair *>(allocate_with_guard(sizeof(typename associative_container<tkey, tvalue>::key_value_pair) * key_value_pairs_max_count));
    node_shell->subtrees = reinterpret_cast<search_tree<tkey, tvalue, tkey_comparer>::search_tree_node **>(allocate_with_guard(sizeof(search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *) * (key_value_pairs_max_count + 1)));
    node_shell->min_keys_count = key_value_pairs_min_count;
    node_shell->involved_keys = 0;

    return node_shell;
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::clone_node_with_subtrees(
    typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node_shell)
{
    if (node_shell == nullptr)
    {
        return nullptr;
    }

    auto *node_shell_copy = create_node_shell(node_shell->min_keys_count);
    auto node_shell_max_keys_count = node_shell->min_keys_count << 1;

    for (auto i = 0; i <= node_shell_max_keys_count; i++)
    {
        if (i != node_shell->min_keys_count)
        {
           new (node_shell_copy->keys_and_values + i) typename associative_container<tkey, tvalue>::key_value_pair(node_shell_copy->keys_and_values[i]);
        }

        // TODO: копирование для B+ должно работать чуть иначе ._.
        node_shell_copy->subtrees[i] = clone_node_with_subtrees(node_shell->subtrees[i]);
    }

    return node_shell_copy;
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::destroy_node(
    search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node_shell,
    bool recursive)
{
    if (node_shell == nullptr)
    {
        return;
    }

    // TODO: удаление узла с поддеревьями для B+ должно работать чуть иначе ._.
    if (recursive)
    {
        for (auto i = 0; i <= node_shell->involved_keys; i++)
        {
            destroy_node(node_shell->subtrees + i, true);
        }
    }
    
    deallocate_with_guard(node_shell->subtrees);
    for (auto i = 0; i < node_shell->involved_keys; i++)
    {
        node_shell->~search_tree_node();
    }
    deallocate_with_guard(node_shell->keys_and_values);
    deallocate_with_guard(node_shell);
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
std::optional<std::pair<typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *, typename associative_container<tkey, tvalue>::key_value_pair> > search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::shift(
    typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node,
    unsigned int index_to_vacate,
    typename search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::node_split_mode split_mode,
    tkey const &key,
    tvalue &&value,
    typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *vacation_right_subtree)
{
    auto node_max_keys_count = node->min_keys_count << 1;
    if (node->involved_keys == node_max_keys_count)
    {
        if (split_mode == search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::node_split_mode::copy_median_kvp && node->subtrees[0] != nullptr)
        {
            split_mode = search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::node_split_mode::do_not_copy_median_kvp;
        }

        auto additional_size = node->subtrees[0] == nullptr
            ? static_cast<int>(split_mode)
            : 0;
        auto *new_node_after_split = create_node_shell(node->min_keys_count);
        auto source_index = node->min_keys_count;

        new_node_after_split->involved_keys = (node->involved_keys >>= 1) + additional_size;

        typename associative_container<tkey, tvalue>::key_value_pair kvps[node_max_keys_count + 1];
        for (auto i = 0; i <= node_max_keys_count; i++)
        {
            if (i == index_to_vacate)
            {
                kvps[i].key = key;
                kvps[i].value = std::move(value);
            }
            else
            {
                kvps[i] = std::move(node->keys_and_values[i - (i < index_to_vacate
                    ? 0
                    : 1)]);
            }
        }

        typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node* subtrees[node_max_keys_count + 2];
        for (auto i = 0; i <= node_max_keys_count; i++)
        {
            if (i == index_to_vacate)
            {
                subtrees[i] = node->subtrees[i];
                subtrees[i + 1] = vacation_right_subtree;
            }
            else
            {
                subtrees[i + (i < index_to_vacate
                    ? 0
                    : 1)] = node->subtrees[i];
            }
        }

        if (node->subtrees[0] == nullptr && split_mode == search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::node_split_mode::copy_median_kvp)
        {
            subtrees[node->min_keys_count] = new_node_after_split;
            //subtrees[node_max_keys_count + 1] = node->subtrees[node->involved_keys];
        }

        for (auto i = 0; i <= node_max_keys_count; i++)
        {
            if (i < node->min_keys_count)
            {
                node->keys_and_values[i] = std::move(kvps[i]);
                node->subtrees[i] = subtrees[i];
            }
            else if (i == node->min_keys_count)
            {
                if (split_mode ==
                         search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::node_split_mode::do_not_copy_median_kvp)
                {
                    node->subtrees[i] = subtrees[i];
                }
                else if (split_mode ==
                    search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::node_split_mode::copy_median_kvp)
                {
                    new (new_node_after_split->keys_and_values) typename search_tree<tkey, tvalue, tkey_comparer>::key_value_pair(
                        kvps[i]);
                    node->subtrees[i] = subtrees[i];
                    new_node_after_split->subtrees[0] = nullptr;
                }
            }
            else if (i > node->min_keys_count)
            {
                auto target_index = i - node->min_keys_count - !static_cast<int>(split_mode);

                new (new_node_after_split->keys_and_values + target_index) typename search_tree<tkey, tvalue, tkey_comparer>::key_value_pair(std::move(kvps[i]));

                new_node_after_split->subtrees[target_index] = subtrees[i];
            }
        }

        new_node_after_split->subtrees[new_node_after_split->min_keys_count + static_cast<int>(split_mode)] = subtrees[node_max_keys_count + 1];

        typename associative_container<tkey, tvalue>::key_value_pair result;
        result.key = split_mode == search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::node_split_mode::copy_median_kvp
            ? std::move(kvps[node->min_keys_count].key)
            : kvps[node->min_keys_count].key;
        result.value = split_mode == search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::node_split_mode::copy_median_kvp
            ? std::move(kvps[node->min_keys_count].value)
            : kvps[node->min_keys_count].value;

        return std::make_optional<std::pair<typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *, typename associative_container<tkey, tvalue>::key_value_pair> >(std::make_pair<typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *, typename associative_container<tkey, tvalue>::key_value_pair>(std::move(new_node_after_split), std::move(result)));
    }
    else
    {
        new (node->keys_and_values + node->involved_keys) typename associative_container<tkey, tvalue>::key_value_pair;

        for (auto i = node->involved_keys; i > index_to_vacate; --i)
        {
            node->keys_and_values[i].key = node->keys_and_values[i - 1].key;
            node->keys_and_values[i].value = std::move(node->keys_and_values[i - 1].value);
            node->subtrees[i + 1] = node->subtrees[i];
        }

        node->keys_and_values[index_to_vacate].key = key;
        node->keys_and_values[index_to_vacate].value = std::move(value);
        node->involved_keys++;
        node->subtrees[index_to_vacate + 1] = vacation_right_subtree;

        return std::nullopt;
    }
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
size_t search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::get_node_shell_size() const noexcept
{
    return sizeof(search_tree<tkey, tvalue, tkey_comparer>::search_tree_node);
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::node_constructor_call(
    typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *allocated_memory) const noexcept
{
    new (allocated_memory) search_tree<tkey, tvalue, tkey_comparer>::search_tree_node;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
void search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::node_additional_data_injector(
    search_tree::search_tree_node *initialized_memory) const noexcept
{

}

// region logger_holder implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
logger *search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::get_logger() const noexcept
{
    return _target->get_logger();
}

// endregion logger_holder implementation

// region memory_holder implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
memory *search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::get_memory() const noexcept
{
    return _target->get_memory();
}

// endregion memory_holder implementation

// endregion search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context implementation

// region exceptions overrides

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
char const *search_tree<tkey, tvalue, tkey_comparer>::insertion_exception::what() const noexcept
{
    return "key to insert by is already exists in tree";
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
char const *search_tree<tkey, tvalue, tkey_comparer>::reading_exception::what() const noexcept
{
    return "key to read by not exists inside tree";
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
char const *search_tree<tkey, tvalue, tkey_comparer>::removing_exception::what() const noexcept
{
    return "key to remove by not exists inside tree";
}

// endregion exception overrides

// region constructors

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
search_tree<tkey, tvalue, tkey_comparer>::search_tree(
    memory *allocator,
    logger *logger,
    node_interaction_context *context)
    : _root(nullptr),
      _allocator(allocator),
      _logger(logger),
      _context(context)
{

}

// endregion constructors

// region rule of 5 implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
search_tree<tkey, tvalue, tkey_comparer>::search_tree(search_tree<tkey, tvalue, tkey_comparer> const &other)
{
    // TODO
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
search_tree<tkey, tvalue, tkey_comparer> &search_tree<tkey, tvalue, tkey_comparer>::operator=(search_tree<tkey, tvalue, tkey_comparer> const &other)
{
    if (this == &other)
    {
        return *this;
    }

    // TODO

    return *this;
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
search_tree<tkey, tvalue, tkey_comparer>::search_tree(search_tree<tkey, tvalue, tkey_comparer> &&other) noexcept
{
    // TODO
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
search_tree<tkey, tvalue, tkey_comparer> &search_tree<tkey, tvalue, tkey_comparer>::operator=(search_tree<tkey, tvalue, tkey_comparer> &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    // TODO

    return *this;
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
search_tree<tkey, tvalue, tkey_comparer>::~search_tree()
{
    // TODO: delete all nodes
    delete _context;
}

// endregion rule of 5 implementation

// region logger_holder implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
logger *search_tree<tkey, tvalue, tkey_comparer>::get_logger() const noexcept
{
    return _logger;
}

// endregion logger_holder implementation

// region memory_holder implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
memory *search_tree<tkey, tvalue, tkey_comparer>::get_memory() const noexcept
{
    return _allocator;
}

// endregion memory_holder implementation

// endregion search_tree<tkey, tvalue, tkey_comparer> implementation

#endif //DATA_STRUCTURES_CPP_SEARCH_TREE_H