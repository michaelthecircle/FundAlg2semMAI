#ifndef DATA_STRUCTURES_CPP_SEARCH_TREE_H
#define DATA_STRUCTURES_CPP_SEARCH_TREE_H

#include <exception>
#include "associative_container.h"
#include "./memory.h"
#include "./logger/logger_holder.h"
#include "./alloc_new_delete/memory_holder.h"

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
class search_tree:
    public associative_container<tkey, tvalue>,
    protected memory_holder,
    protected logger_holder
{

protected:

    struct search_tree_node
    {

        typename associative_container<tkey, tvalue>::key_value_pair *keys_and_values;
        search_tree_node **subtrees;
        unsigned int involved_keys;

    public:

        virtual ~search_tree_node() noexcept = default;

    };

protected:

    class node_interaction_context:
        protected memory_holder,
        protected logger_holder
    {

    private:

        search_tree<tkey, tvalue, tkey_comparer> *_target;

    public:

        node_interaction_context(
            search_tree<tkey, tvalue, tkey_comparer> *target)
            : _target(target)
        {

        }

    public:

        typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *create_node(
            unsigned int node_size);

        typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *clone_node_with_subtrees(
            typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node_shell);

        void destroy_node(
            typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node_shell);

        void destroy_node_with_subtrees(
            typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node_shell);

        void shift_without_split(
            typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node,
            unsigned int index_to_vacate);

        std::pair<search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *, tkey> shift_with_split(
            typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node,
            unsigned int index_to_vacate,
            tkey const &vacation_shell_key,
            tvalue &&vacation_shell_value);

    protected:

        virtual size_t get_node_shell_size() const noexcept;

        virtual void node_constructor_call(
            search_tree_node *allocated_memory) const noexcept;

        virtual void node_additional_data_injector(
            search_tree_node *initialized_memory) const noexcept;

    protected:

        memory *get_memory() const noexcept final { return _target->get_memory(); }

    protected:

        logger *get_logger() const noexcept final { return _target->get_logger(); }

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

protected:

    search_tree_node *_root;
    logger *_logger;
    memory *_allocator;
    node_interaction_context *_context;

protected:

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

private:

    memory *get_memory() const noexcept override
    {
        return _allocator;
    }

private:

    logger *get_logger() const noexcept override
    {
        return _logger;
    }

};

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::create_node(
    unsigned int node_size)
{
    auto *node_shell = reinterpret_cast<search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *>(allocate_with_guard(get_node_shell_size()));
    node_shell->keys_and_values = reinterpret_cast<typename associative_container<tkey, tvalue>::key_value_pair *>(allocate_with_guard(sizeof(typename associative_container<tkey, tvalue>::key_value_pair) * node_size));
    for (auto i = 0; i < node_size; i++)
    {
        new (node_shell->keys_and_values + i) typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node;
    }
    node_shell->subtrees = reinterpret_cast<search_tree<tkey, tvalue, tkey_comparer>::search_tree_node **>(allocate_with_guard(sizeof(search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *) * (node_size + 1)));
    memset(node_shell->subtrees, 0, sizeof(search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *) * (node_size + 1));
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
    auto *node_shell_copy = reinterpret_cast<search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *>(allocate_with_guard(get_node_shell_size()));



    return node_shell;
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::destroy_node(
    search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node_shell)
{
    deallocate_with_guard(node_shell->subtrees);
    for (auto i = 0; i < node_shell->involved_keys; i++)
    {
        node_shell->keys_and_values[i].key.~tkey();
        node_shell->keys_and_values[i].key.~tvalue();
    }
    deallocate_with_guard(node_shell->keys_and_values);
    deallocate_with_guard(node_shell);
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::shift_without_split(
    typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node,
    unsigned int index_to_vacate)
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
std::pair<typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *, tkey> search_tree<tkey, tvalue, tkey_comparer>::node_interaction_context::shift_with_split(
    typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node,
    unsigned int index_to_vacate,
    tkey const &vacation_shell_key,
    tvalue &&vacation_shell_value)
{
    throw std::exception();
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

// region

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

#endif //DATA_STRUCTURES_CPP_SEARCH_TREE_H