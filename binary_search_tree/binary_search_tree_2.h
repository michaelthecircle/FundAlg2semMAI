#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stack>
#include <list>
#include "./logger/logger_concrete.h"
#include "./alloc_new_delete/memory_holder.h"
#include <vector>
#include <sstream>
#include "./b_plus_tree/associative_container.h"
#include "not_implemented.h"
#include "logger/logger.h"
#include "logger/logger_holder.h"
#include "./memory.h"

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
class binary_search_tree : public associative_container<tkey, tvalue>,
                           private logger_holder,
                           protected memory_holder
{

protected:
    struct node
    {
        tkey key;

        tvalue value;

        node *left_subtree_address;

        node *right_subtree_address;

        virtual ~node() = default;
    };

public:
    class prefix_iterator final
    {

    private:
        node *_current_node;
        std::stack<node *> _way;

    public:
        explicit prefix_iterator(
                node *tree_root);

    public:
        bool operator==(
                prefix_iterator const &other) const;

        bool operator!=(prefix_iterator const &other) const;

        prefix_iterator &operator++();

        prefix_iterator operator++(
                int not_used);

        std::tuple<unsigned int, tkey const &, tvalue const &> operator*() const;
    };

    class infix_iterator final
    {

    private:
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *_current_node;
        std::stack<node *> _way;

    public:
        explicit infix_iterator(
                binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree);

    public:
        bool operator==(
                infix_iterator const &other) const;

        infix_iterator &operator++();

        bool operator!=(infix_iterator const &other) const;

        infix_iterator operator++(
                int not_used);

        std::tuple<unsigned int, tkey const &, tvalue const &> operator*() const;
    };

    class postfix_iterator final
    {
        friend class binary_search_tree<tkey, tvalue, tkey_comparer>;

    private:
        node *_current_node;
        std::stack<node *> _way;

    public:
        explicit postfix_iterator(
                node *tree_root);

    public:
        bool operator==(
                postfix_iterator const &other) const;

        postfix_iterator &operator++();

        postfix_iterator operator++(
                int not_used);

        bool operator!=(postfix_iterator const &other) const;

        std::tuple<unsigned int, tkey const &, tvalue const &> operator*() const;

    private:
        node *get_current_node() const;
    };

protected:
    class insertion_template_method : protected memory_holder, protected logger_holder
    {

        friend class binary_search_tree<tkey, tvalue, tkey_comparer>;

    public:
        explicit insertion_template_method(binary_search_tree<tkey, tvalue, tkey_comparer> *tree)
                : _tree(tree)
        {
        }

        virtual ~insertion_template_method() = default;

    public:
        void insert(tkey const &key, tvalue &&value, node *&tree_root_address);

    private:
        void insert_inner(
                tkey const &key,
                tvalue &&value,
                node *&subtree_root_address,
                std::stack<node **> &path_to_subtree_root_exclusive);

    protected:
        virtual void before_insert() {}

        virtual void after_insert_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive);

        void initialize_node(node *mem, tkey const &key, tvalue &&value) const;

        virtual size_t get_node_size() const;

        virtual void call_constructor_node(node *mem) const;

        virtual void initialize_node_additional_data(node *mem) const;

        virtual void inject_additional_data(
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *from,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *to) const;

    private:
        memory *get_memory() const noexcept final;

        logger *get_logger() const noexcept final;

    private:
        binary_search_tree<tkey, tvalue, tkey_comparer> *_tree;
    };

    // friend class insertion_template_method;

    class reading_template_method : protected logger_holder
    {
    private:
        binary_search_tree<tkey, tvalue, tkey_comparer> *_tree;

    public:
        reading_template_method(binary_search_tree<tkey, tvalue, tkey_comparer> *tree)
                : _tree(tree)
        {
        }

    public:
        bool read(
                typename associative_container<tkey, tvalue>::key_value_pair *target_key_and_result_value,
                node *&tree_root_address);

    private:
        bool read_inner(
                typename associative_container<tkey, tvalue>::key_value_pair *target_key_and_result_value,
                node *&subtree_root_address,
                std::stack<node **> &path_to_subtree_root_exclusive);

    protected:
        virtual void before_read_inner(
                typename associative_container<tkey, tvalue>::key_value_pair *target_key_and_result_value,
                node *&subtree_root_address,
                std::stack<node **> &path_to_subtree_root_exclusive);

        virtual void after_read_inner(
                typename associative_container<tkey, tvalue>::key_value_pair *target_key_and_result_value,
                node *&subtree_root_address,
                std::stack<node **> &path_to_subtree_root_exclusive);

    protected:
        logger *get_logger() const noexcept final;
    };

    class removing_template_method : protected logger_holder, protected memory_holder
    {
    private:
        binary_search_tree<tkey, tvalue, tkey_comparer> *_tree;

    public:
        explicit removing_template_method(binary_search_tree<tkey, tvalue, tkey_comparer> *tree)
                : _tree(tree)
        {
        }

    public:
        tvalue remove(
                tkey const &key,
                node *&tree_root_address);

    private:
        tvalue remove_inner(
                tkey const &key,
                node *&subtree_root_address,
                std::list<node **> &path_to_subtree_root_exclusive);

    protected:
        virtual void before_remove();

    protected:
        virtual void before_balance(
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *node_to_remove_parent,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *node_to_remove);

        virtual void after_remove_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive);

    private:
        template <typename T>
        void swap(T **value, T **another_value)
        {
            T *value_ptr_copy = *value;
            *value = *another_value;
            *another_value = value_ptr_copy;
        }

        void swap_nodes(node **one_node, node **another_node);

    protected:
        virtual void swap_additional_nodes_data(node *one_node, node *another_node);

    private:
        memory *get_memory() const noexcept final;

        logger *get_logger() const noexcept final;
    };

protected:
    memory *_allocator;
    logger *_logger;
    node *_root;
    insertion_template_method *_insertion;
    reading_template_method *_reading;
    removing_template_method *_removing;

private:
    tkey_comparer _comparator;

protected:
    node *get_root() const noexcept { return _root; }

protected:
    explicit binary_search_tree(
            insertion_template_method *insertion,
            reading_template_method *reading,
            removing_template_method *removing,
            memory *allocator = nullptr,
            logger *logger = nullptr);

public:
    explicit binary_search_tree(
            memory *allocator = nullptr,
            logger *logger = nullptr);

    binary_search_tree(
            binary_search_tree const &other);

    binary_search_tree(
            binary_search_tree &&other) noexcept;

    binary_search_tree &operator=(
            binary_search_tree const &other);

    binary_search_tree &operator=(
            binary_search_tree &&other) noexcept;

    ~binary_search_tree();

public:
    void insert(
            tkey const &key,
            tvalue &&value) override;

    bool find(
            typename associative_container<tkey, tvalue>::key_value_pair *target_key_and_result_value) final;

    tvalue remove(
            tkey const &key) override;

private:
    logger *get_logger() const noexcept final;

    memory *get_memory() const noexcept final;

protected:
    node *copy() const;

    void move(binary_search_tree<tkey, tvalue, tkey_comparer> &&other);

    void clear();

private:
    node *copy_inner(node *to_copy) const;

    void clear_inner(node *to_clear) const;

public:
    prefix_iterator begin_prefix() const noexcept;

    prefix_iterator end_prefix() const noexcept;

    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    postfix_iterator begin_postfix() const noexcept;

    postfix_iterator end_postfix() const noexcept;

protected:
    void left_rotation(binary_search_tree<tkey, tvalue, tkey_comparer>::node **subtree_root) const;
    void right_rotation(binary_search_tree<tkey, tvalue, tkey_comparer>::node **subtree_root) const;
};

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::left_rotation(binary_search_tree<tkey, tvalue, tkey_comparer>::node **subtree_root) const
{
    node *tmp = *subtree_root;
    *subtree_root = (*subtree_root)->right_subtree_address;
    tmp->right_subtree_address = (*subtree_root)->left_subtree_address;
    (*subtree_root)->left_subtree_address = tmp;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::right_rotation(binary_search_tree<tkey, tvalue, tkey_comparer>::node **subtree_root) const
{
    node *tmp = *subtree_root;
    *subtree_root = (*subtree_root)->left_subtree_address;
    tmp->left_subtree_address = (*subtree_root)->right_subtree_address;
    (*subtree_root)->right_subtree_address = tmp;
}

// region iterators implementation

// region prefix_iterator implementation

// --------------------------------------------------------------------------------------------------------------------------

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::prefix_iterator(
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree_root) : _current_node(tree_root)
{
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator==(
        binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator const &other) const
{
    if (_way == other._way && _current_node == other._current_node)
    {
        return true;
    }

    return false;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator &binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator++()
{
    if (_current_node == nullptr)
    {
        return *this;
    }

    if (_current_node->left_subtree_address != nullptr)
    {
        _way.push(_current_node);
        _current_node = _current_node->left_subtree_address;

        return *this;
    }
    else if (_current_node->right_subtree_address != nullptr)
    {
        _way.push(_current_node);
        _current_node = _current_node->right_subtree_address;
        return *this;
    }
    else
    {
        while (true)
        {
            if (_way.empty())
            {
                _current_node = nullptr;
                return *this;
            }
            else if (_way.top()->right_subtree_address == _current_node)
            {
                _current_node = _way.top();
                _way.pop();
            }
            else
            {
                if (_way.top()->right_subtree_address != nullptr)
                {
                    _current_node = _way.top()->right_subtree_address;
                    return *this;
                }
                else
                {
                    _current_node = _way.top()->right_subtree_address;
                }
            }
        }
    }
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator++(
        int not_used)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator prev_state = *this;

    ++(*this);

    return prev_state;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator!=(
        prefix_iterator const &other) const
{
    return !(*this == other);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
std::tuple<unsigned int, tkey const &, tvalue const &> binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator*() const
{
    return std::tuple<unsigned int, tkey const &, tvalue const &>(_way.size(), _current_node->key, _current_node->value);
}

// endregion prefix_iterator implementation

// region infix_iterator implementation

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::infix_iterator(
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree)
{
    _current_node = tree;
    while (_current_node && _current_node->left_subtree_address)
    {
        _way.push(_current_node);
        _current_node = _current_node->left_subtree_address;
    }
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator==(
        binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator const &other) const
{

    if (_way == other._way && _current_node == other._current_node)
    {
        return true;
    }
    return false;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator!=(binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator const &other) const
{
    return !(*this == other);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator &binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator++()
{

    if (_current_node == nullptr)
    {
        return *this;
    }

    if (_current_node->right_subtree_address != nullptr)
    {
        _way.push(_current_node);
        _current_node = _current_node->right_subtree_address;

        while (_current_node->left_subtree_address != nullptr)
        {
            _way.push(_current_node);
            _current_node = _current_node->left_subtree_address;
        }
    }
    else
    {
        while (!_way.empty())
        {
            if (_way.top()->left_subtree_address == _current_node)
            {
                // _current_node = _way.top();
                // _way.pop();
                break;
            }
            _current_node = _way.top();
            _way.pop();
        }

        if (!_way.empty())
        {
            _current_node = _way.top();
            _way.pop();
        }
        else
        {
            _current_node = nullptr;
        }
        // {
        //     while (!_way.empty() && _way.top()->right_subtree_address == _current_node)
        //     {
        //         _current_node = _way.top();
        //         _way.pop();
        //     }

        //     if (_way.empty())
        //     {
        //         _current_node = nullptr;
        //         return *this;
        //     }

        //     _current_node = _way.top();
        //     _way.pop();
        // }
    }
    return *this;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator++(
        int not_used)
{
    auto prev_state = *this;

    ++(*this);

    return prev_state;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
std::tuple<unsigned int, tkey const &, tvalue const &> binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator*() const
{
    return std::tuple<unsigned int, tkey const &, tvalue const &>(_way.size(), _current_node->key, _current_node->value);
}

// endregion infix_iterator implementation

// region postfix_iterator implementation

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::postfix_iterator(
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree_root)
{
    _current_node = tree_root;
    // while (_current_node != nullptr && _current_node->left_subtree_address != nullptr)
    // {
    //     _way.push(_current_node);
    //     _current_node = _current_node->left_subtree_address;
    // }

    // if (_current_node == nullptr)
    // {
    //     return;
    // }

    while (_current_node != nullptr && (_current_node->left_subtree_address != nullptr || _current_node->right_subtree_address != nullptr))
    {
        _way.push(_current_node);
        _current_node = _current_node->left_subtree_address == nullptr
                        ? _current_node->right_subtree_address
                        : _current_node->left_subtree_address;
    }
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator==(
        binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator const &other) const
{
    if (_way == other._way && _current_node == other._current_node)
    {
        return true;
    }

    return false;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator!=(
        binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator const &other) const
{
    return !(*this == other);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator &binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator++()
{
    if (_current_node == nullptr)
    {
        return *this;
    }

    if (_way.empty())
    {
        _current_node = nullptr;
    }
    else
    {
        if (_way.top()->right_subtree_address == _current_node)
        {
            _current_node = _way.top();
            _way.pop();
        }
        else
        {
            if (_way.top()->right_subtree_address != nullptr)
            {
                _current_node = _way.top()->right_subtree_address;
                while (_current_node->left_subtree_address != nullptr || _current_node->right_subtree_address != nullptr)
                {
                    _way.push(_current_node);
                    _current_node = _current_node->left_subtree_address != nullptr ? _current_node->left_subtree_address
                                                                                   : _current_node->right_subtree_address;
                }
            }
            else
            {
                _current_node = _way.top();
                _way.pop();
            }
        }
    }
    return *this;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator++(
        int not_used)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator prev_state = *this;

    ++(*this);

    return prev_state;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
std::tuple<unsigned int, tkey const &, tvalue const &> binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator*() const
{
    return std::tuple<unsigned int, tkey const &, tvalue const &>(_way.size(), _current_node->key, _current_node->value);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::get_current_node() const
{
    return _current_node;
}

// endregion prefix_iterator implementation

// endregion iterators implementation

// region template methods implementation

// region insertion implementation

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::insert(
        tkey const &key,
        tvalue &&value,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&tree_root_address)
{
    before_insert();
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> path_to_subtree_root_exclusive;
    insert_inner(key, std::move(value), tree_root_address, path_to_subtree_root_exclusive);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
size_t binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::get_node_size() const
{
    return sizeof(binary_search_tree<tkey, tvalue, tkey_comparer>::node);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::call_constructor_node(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *mem) const
{
    new (mem) binary_search_tree<tkey, tvalue, tkey_comparer>::node;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::initialize_node(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *mem,
        tkey const &key,
        tvalue &&value) const
{
    call_constructor_node(mem);
    mem->key = key;
    mem->value = std::move(value);
    mem->left_subtree_address = mem->right_subtree_address = nullptr;
    initialize_node_additional_data(mem);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::initialize_node_additional_data(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *mem) const
{
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::inject_additional_data(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *from,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *to) const
{
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::insert_inner(
        tkey const &key,
        tvalue &&value,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    if (subtree_root_address == nullptr)
    {
        subtree_root_address = reinterpret_cast<node *>(allocate_with_guard(get_node_size()));

        // this->trace_with_guard("Allocated root");

        initialize_node(subtree_root_address, key, std::move(value));
    }
    else
    {
        int res_functor = _tree->_comparator(key, subtree_root_address->key);
        if (res_functor == 0)
        {
            // TODO: ALL_MY_EXCEPTION_TYPE
            throw std::exception();
        }

        path_to_subtree_root_exclusive.push(&subtree_root_address);

        insert_inner(key, std::move(value), res_functor > 0 ? subtree_root_address->right_subtree_address : subtree_root_address->left_subtree_address, path_to_subtree_root_exclusive);
        path_to_subtree_root_exclusive.pop();
    }
    after_insert_inner(key, subtree_root_address, path_to_subtree_root_exclusive);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::after_insert_inner(
        tkey const &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
memory *binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::get_memory() const noexcept
{
    return _tree->get_memory();
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
logger *binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::get_logger() const noexcept
{
    return _tree->get_logger();
}

// endregion insertion implementation

// region reading implementation

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::read(
        typename associative_container<tkey, tvalue>::key_value_pair *target_key_and_result_value,
        node *&tree_root_address)
{
    std::stack<binary_search_tree::node **> path_to_subtree_root_exclusive;
    return read_inner(target_key_and_result_value, tree_root_address, path_to_subtree_root_exclusive);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::read_inner(
        typename associative_container<tkey, tvalue>::key_value_pair *target_key_and_result_value,
        node *&subtree_root_address,
        std::stack<node **> &path_to_subtree_root_exclusive)
{
    if (subtree_root_address == nullptr)
    {
        this->trace_with_guard("Key not found");
    }

    tkey_comparer comparator;

    auto find_node = &subtree_root_address;

    while (*find_node != nullptr)
    {
        int compare_result = comparator(target_key_and_result_value->key, (*find_node)->key);

        if (compare_result == 0)
        {
            target_key_and_result_value->value = std::move((*find_node)->value);
            after_read_inner(target_key_and_result_value, subtree_root_address, path_to_subtree_root_exclusive);
            return true;
        }
        else
        {
            path_to_subtree_root_exclusive.push(find_node);
            find_node = &(compare_result > 0 ? (*find_node)->right_subtree_address
                                             : (*find_node)->left_subtree_address);
        }
    }

    return false;

    // before_read_inner(key, subtree_root_address, path_to_subtree_root_exclusive);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::before_read_inner(
        typename associative_container<tkey, tvalue>::key_value_pair *target_key_and_result_value,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::after_read_inner(
        typename associative_container<tkey, tvalue>::key_value_pair *target_key_and_result_value,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
logger *binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::get_logger() const noexcept
{
    return _tree->_logger;
}

// endregion reading implementation

// region removing implementation

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
tvalue binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::remove(
        tkey const &key,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&tree_root_address)
{
    before_remove();
    std::list<binary_search_tree::node **> path_to_subtree_root_exclusive;
    return remove_inner(key, tree_root_address, path_to_subtree_root_exclusive);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::before_remove()
{
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::before_balance(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *node_to_remove_parent,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *node_to_remove)
{
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
tvalue binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::remove_inner(
        tkey const &key,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::list<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    if (subtree_root_address == nullptr)
    {
        this->warning_with_guard("Tree is empty");

        throw std::exception();
    }

    auto current_node = &subtree_root_address;

    node **removed_node = nullptr;

    while (*current_node != nullptr)
    {
        int compare_result = _tree->_comparator(key, (*current_node)->key);

        if (compare_result == 0)
        {
            removed_node = current_node;

            break;
        }

        path_to_subtree_root_exclusive.push_back(current_node);
        current_node = &(compare_result > 0
                         ? (*current_node)->right_subtree_address
                         : (*current_node)->left_subtree_address);
    }

    if (removed_node == nullptr)
    {
        this->warning_with_guard("Key not found");

        throw std::exception();
    }

    tvalue removed_value = std::move((*removed_node)->value);

    auto removal_completed = false;

    if ((*removed_node)->left_subtree_address != nullptr && (*removed_node)->right_subtree_address != nullptr)
    {
        auto successor = (*removed_node)->right_subtree_address;

        if (successor->left_subtree_address == nullptr)
        {
            successor->left_subtree_address = (*removed_node)->left_subtree_address;
            (*removed_node)->~node();
            deallocate_with_guard(*removed_node);
            *removed_node = successor;
            removal_completed = true;
        }
        else
        {
            path_to_subtree_root_exclusive.push_back(removed_node);
            path_to_subtree_root_exclusive.push_back(&(*removed_node)->right_subtree_address);

            while (successor->left_subtree_address != nullptr)
            {
                path_to_subtree_root_exclusive.push_back(&(successor->left_subtree_address));
                successor = successor->left_subtree_address;
            }

            auto removed_node_initial = removed_node;
            removed_node = *path_to_subtree_root_exclusive.rbegin();
            path_to_subtree_root_exclusive.pop_back();

            tkey to_swap_keys = (*removed_node)->key;
            (*removed_node)->key = (*removed_node_initial)->key;
            (*removed_node_initial)->key = to_swap_keys;

            tvalue to_swap_values = std::move((*removed_node)->value);
            (*removed_node)->value = (std::move((*removed_node_initial)->key));
            (*removed_node_initial)->value = std::move(to_swap_values);
        }
    }

    if (!removal_completed)
    {
        if ((*removed_node)->left_subtree_address == nullptr && (*removed_node)->right_subtree_address == nullptr)
        {
            (*removed_node)->~node();
            deallocate_with_guard((*removed_node));
            (*removed_node) = nullptr;
        }
        else
        {
            auto tmp = (*removed_node)->left_subtree_address != nullptr
                       ? (*removed_node)->left_subtree_address
                       : (*removed_node)->right_subtree_address;
            (*removed_node)->~node();
            deallocate_with_guard((*removed_node));
            (*removed_node) = tmp;
        }
    }

    this->trace_with_guard("Node with key: {" + std::to_string(key) + "} has been deleted.");

    after_remove_inner(key, subtree_root_address, path_to_subtree_root_exclusive);

    return removed_value;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::after_remove_inner(
        tkey const &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::swap_additional_nodes_data(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *one_node, typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *another_node)
{
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::swap_nodes(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **one_node, typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **another_node)
{
    swap(one_node, another_node);
    swap(&(*one_node)->left_subtree_address, &(*another_node)->left_subtree_address);
    swap(&(*one_node)->right_subtree_address, &(*another_node)->right_subtree_address);
    swap_additional_nodes_data(*one_node, *another_node);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
logger *binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::get_logger() const noexcept
{
    return _tree->_logger;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
memory *binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::get_memory() const noexcept
{
    return _tree->_allocator;
}

// endregion implementation

// endregion template methods

// region construction, assignment, destruction implementation

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::move(binary_search_tree<tkey, tvalue, tkey_comparer> &&other)
{
    _root = other._root;
    other._root = nullptr;

    _allocator = other._allocator;
    other._allocator = nullptr;

    _logger = other._logger;
    other._logger = nullptr;

    _insertion = other._insertion;
    other._insertion = nullptr;

    _reading = other._reading;
    other._reading = nullptr;

    _removing = other._removing;
    other._removing = nullptr;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *binary_search_tree<tkey, tvalue, tkey_comparer>::copy() const
{
    return copy_inner(_root);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *binary_search_tree<tkey, tvalue, tkey_comparer>::copy_inner(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *to_copy) const
{
    if (to_copy == nullptr)
    {
        return nullptr;
    }

    auto *node_copy = reinterpret_cast<binary_search_tree<tkey, tvalue, tkey_comparer>::node *>(allocate_with_guard(_insertion->get_node_size()));

    _insertion->initialize_node(node_copy, to_copy->key, std::move(tvalue(to_copy->value)));
    _insertion->inject_additional_data(to_copy, node_copy);

    node_copy->left_subtree_address = copy_inner(to_copy->left_subtree_address);
    node_copy->right_subtree_address = copy_inner(to_copy->right_subtree_address);

    return node_copy;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::clear()
{
    clear_inner(_root);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::clear_inner(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *to_clear) const
{
    if (to_clear == nullptr)
    {
        return;
    }

    clear_inner(to_clear->left_subtree_address);
    clear_inner(to_clear->right_subtree_address);

    to_clear->~node();
    deallocate_with_guard(to_clear);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method *insertion,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method *reading,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method *removing,
        memory *allocator,
        logger *logger)
        : _insertion(insertion),
          _reading(reading),
          _removing(removing),
          _allocator(allocator),
          _logger(logger),
          _root(nullptr),
          _comparator()
{
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
        memory *allocator,
        logger *logger) : binary_search_tree(new insertion_template_method(this), new reading_template_method(this), new removing_template_method(this), allocator, logger)
{
    this->trace_with_guard("The tree has been created.");
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
        const binary_search_tree<tkey, tvalue, tkey_comparer> &other) : binary_search_tree<tkey, tvalue, tkey_comparer>(other._allocator, other._logger)
{
    _root = other.copy();

    this->trace_with_guard("The tree has been copy");
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
        binary_search_tree<tkey, tvalue, tkey_comparer> &&other) noexcept
{
    move(std::move(other));
    this->trace_with_guard("The tree has been moved.");
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer> &binary_search_tree<tkey, tvalue, tkey_comparer>::operator=(
        const binary_search_tree<tkey, tvalue, tkey_comparer> &other)
{
    if (this == &other)
    {
        return *this;
    }

    clear();

    _allocator = other._allocator;
    _logger = other._logger;

    _root = other.copy();
    return *this;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer> &binary_search_tree<tkey, tvalue, tkey_comparer>::operator=(
        binary_search_tree<tkey, tvalue, tkey_comparer> &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    clear();

    delete _insertion;
    delete _reading;
    delete _removing;

    move(std::move(other));

    return *this;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::~binary_search_tree()
{
    clear();

    delete _insertion;
    delete _reading;
    delete _removing;

    this->trace_with_guard("The tree has been deleted.");
}

// endregion construction, assignment, destruction implementation

// region associative_container contract implementation

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insert(
        tkey const &key,
        tvalue &&value)
{
    _insertion->insert(key, std::move(value), _root);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::find(
        typename associative_container<tkey, tvalue>::key_value_pair *target_key_and_result_value)
{
    return _reading->read(target_key_and_result_value, _root);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
tvalue binary_search_tree<tkey, tvalue, tkey_comparer>::remove(
        tkey const &key)
{
    return _removing->remove(key, _root);
}

// endregion associative_container contract implementations

// region logger_holder contract implementation

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
logger *binary_search_tree<tkey, tvalue, tkey_comparer>::get_logger() const noexcept
{
    return _logger;
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
memory *binary_search_tree<tkey, tvalue, tkey_comparer>::get_memory() const noexcept
{
    return _allocator;
}

// endregion logger_holder contract implementation

// region iterators requesting implementation

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::begin_prefix() const noexcept
{
    return prefix_iterator(_root);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::end_prefix() const noexcept
{
    return prefix_iterator(nullptr);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::begin_infix() const noexcept
{
    return infix_iterator(_root);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::end_infix() const noexcept
{
    return infix_iterator(nullptr);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::begin_postfix() const noexcept
{
    return postfix_iterator(_root);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::end_postfix() const noexcept
{
    return postfix_iterator(nullptr);
}

#endif // BINARY_SEARCH_TREE_H
