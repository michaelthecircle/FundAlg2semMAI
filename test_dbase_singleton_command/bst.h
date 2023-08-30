#ifndef MAIN_CPP_BST_H
#define MAIN_CPP_BST_H
#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "../b_plus_tree/associative_container.h"
#include "../logger/logger.h"
#include "../logger/logger_holder.h"
#include "../memory.h"
#include "../alloc_new_delete/memory_holder.h"
#include <vector>
#include <stack>
#include <sstream>
#include <list>

template <typename T>
std::string to_string(T str)
{
    return std::string { (std::stringstream () << str).str() };
}


template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
class binary_search_tree :
        public associative_container<tkey, tvalue>,
        protected logger_holder,
        protected memory_holder
{

protected:

    struct node
    {
        typename associative_container<tkey, tvalue>::key_value_pair key_and_value;

        node * left_subtree_address;

        node * right_subtree_address;

    public:

        virtual ~node() = default;

    };

public:

    class prefix_iterator final
    {

    private:

        node * _current_node;
        std::stack<node *> _way;

    public:

        explicit prefix_iterator(node *tree_root);

    public:

        bool operator==(prefix_iterator const &other) const;

        bool operator!=(prefix_iterator const &other) const;

        prefix_iterator& operator++();

        prefix_iterator operator++(int not_used);

        std::tuple<unsigned int, tkey const&, tvalue const&> operator*() const;

    };

    class infix_iterator final
    {

    private:

        node * _current_node;
        std::stack<node *> _way;

    public:

        explicit infix_iterator(node *tree_root);

    public:

        bool operator==(infix_iterator const &other) const;

        bool operator!=(infix_iterator const &other) const;

        infix_iterator& operator++();

        infix_iterator operator++(int not_used);

        std::tuple<unsigned int, tkey const&, tvalue const&> operator*() const;

    };

    class postfix_iterator final
    {

        friend class binary_search_tree<tkey, tvalue, tkey_comparer>;

    private:

        node * _current_node;
        std::stack<node *> _way;

    public:

        explicit postfix_iterator(node *tree_root);

    public:

        bool operator==(postfix_iterator const &other) const;

        bool operator!=(postfix_iterator const &other) const;

        postfix_iterator &operator++();

        postfix_iterator operator++(int not_used);

        std::tuple<unsigned int, tkey const&, tvalue const&> operator*() const;

    };

protected:

    class insertion_template_method : protected logger_holder, protected memory_holder
    {

    public:

        binary_search_tree<tkey, tvalue, tkey_comparer> * _tree;

    public:

        explicit insertion_template_method(binary_search_tree<tkey, tvalue, tkey_comparer> *tree);

        virtual ~insertion_template_method() = default;

    public:

        void insert(
                tkey const &key,
                tvalue &&value,
                node *&subtree_root_address);

    private:

        void insert_inner(
                tkey const &key,
                tvalue &&value,
                node *&subtree_root_address,
                std::stack<node **> &path_to_subtree_root_exclusive);

    protected:

        virtual void before_insert_inner(
                tkey const &key,
                node *&subtree_root_address,
                std::stack<node **> &path_to_subtree_root_exclusive);

        virtual void after_insert_inner(
                tkey const &key,
                node *&subtree_root_address,
                std::stack<node **> &path_to_subtree_root_exclusive);

        virtual size_t get_node_size() const;

        virtual void call_node_constructor(binary_search_tree<tkey, tvalue, tkey_comparer>::node * node_address) const;

        virtual void initialize_additional_data(binary_search_tree<tkey, tvalue, tkey_comparer>::node * node_address) const;

    private:

        void initialize_node(binary_search_tree<tkey, tvalue, tkey_comparer>::node * const node_address, tkey const &key, tvalue &&value) const;

        logger * get_logger() const override;

        memory * get_memory() const noexcept override
        {
            return _tree->_allocator;
        }
    };

    class reading_template_method : protected logger_holder
    {

    public:

        binary_search_tree<tkey, tvalue, tkey_comparer> * _tree;

    public:

        explicit reading_template_method(binary_search_tree<tkey, tvalue, tkey_comparer> *tree);

        virtual ~reading_template_method() = default;

    public:

        bool read(
                typename associative_container<tkey, tvalue>::key_value_pair * target_key_and_result_value,
                node *&subtree_root_address);

    private:

        bool read_inner(
                typename associative_container<tkey, tvalue>::key_value_pair * target_key_and_result_value,
                node *&subtree_root_address,
                std::stack<node **> &path_to_subtree_root_exclusive);

    protected:

        virtual void before_read_inner(
                typename associative_container<tkey, tvalue>::key_value_pair * target_key_and_result_value,
                node *&subtree_root_address,
                std::stack<node **> &path_to_subtree_root_exclusive);

        virtual void after_read_inner(
                typename associative_container<tkey, tvalue>::key_value_pair * target_key_and_result_value,
                node *&subtree_root_address,
                std::stack<node **> &path_to_subtree_root_exclusive);

    private:

        logger * get_logger() const override;
    };

    class removing_template_method : protected logger_holder, protected memory_holder
    {

    public:

        binary_search_tree<tkey, tvalue, tkey_comparer> * _tree;

    public:

        explicit removing_template_method(binary_search_tree<tkey, tvalue, tkey_comparer> *tree);

        virtual ~removing_template_method() = default;

    public:

        tvalue remove(
                tkey const &key,
                node *&tree_root_address);

    protected:

        virtual tvalue remove_inner(
                tkey const &key,
                node *&subtree_root_address,
                std::stack<node **> &path_to_subtree_root_exclusive);

    protected:

        virtual void before_remove_inner(
                tkey const &key,
                node *&subtree_root_address,
                std::stack<node **> &path_to_subtree_root_exclusive);

        virtual void after_remove_inner(
                tkey const &key,
                node *&subtree_root_address,
                std::stack<node **> &path_to_subtree_root_exclusive);

    private:

        logger * get_logger() const override;

        memory * get_memory() const noexcept override
        {
            return _tree->_allocator;
        }

    protected:

        virtual void swap_additional_nodes_data(binary_search_tree::node *one_node, binary_search_tree::node *another_node);

    };

protected:

    node *_root;
    memory *_allocator;
    logger *_logger;
    tkey_comparer _comparator;
    insertion_template_method *_insertion;
    reading_template_method *_reading;
    removing_template_method *_removing;

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

    void insert(tkey const &key, tvalue &&value) override final;

    bool find(typename associative_container<tkey, tvalue>::key_value_pair * target_key_and_result_value) override final;

    tvalue remove(tkey const &key) override final;

protected:

    memory * get_memory() const noexcept override
    {
        return _allocator;
    }

    logger * get_logger() const override;

protected:

    virtual size_t get_node_size() const;

protected:

    node *copy() const;

    virtual node *copy_inner(node *to_copy) const;

    void move(binary_search_tree<tkey, tvalue, tkey_comparer> &&other);

    void clear_inner(node *to_clear);

    void clear();

public:

    prefix_iterator begin_prefix() const noexcept;

    prefix_iterator end_prefix() const noexcept;

    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    postfix_iterator begin_postfix() const noexcept;

    postfix_iterator end_postfix() const noexcept;


protected:

    virtual void right_rotation(node *&subtree_root) const;

    virtual void left_rotation(node *&subtree_root) const;

};

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::left_rotation(node *&subtree_root) const
{
    node * tmp = subtree_root;
    subtree_root = subtree_root->right_subtree_address;
    tmp->right_subtree_address = subtree_root->left_subtree_address;
    subtree_root->left_subtree_address = tmp;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::right_rotation(node *&subtree_root) const
{
    node *tmp = subtree_root;
    subtree_root = subtree_root->left_subtree_address;
    tmp->left_subtree_address = subtree_root->right_subtree_address;
    subtree_root->right_subtree_address = tmp;
}
// region iterators implementation

//region prefix_iterator implementation

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::prefix_iterator(
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree_root):
        _current_node(tree_root)
{

}

template<
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

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator!=(
        prefix_iterator const &other) const
{
    return !(*this == other);
}

template<
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

template<
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

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
std::tuple<unsigned int, tkey const&, tvalue const&> binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator*() const
{
    return std::tuple<unsigned int, tkey const&, tvalue const&>(_way.size(), _current_node->key_and_value.key, _current_node->key_and_value.value);
}

//endregion prefix_iterator implementation

//region infix_iterator implementation

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::infix_iterator(
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree_root)
{
    _current_node = tree_root;
    while (_current_node && _current_node->left_subtree_address)
    {
        _way.push(_current_node);
        _current_node = _current_node->left_subtree_address;
    }
}

template<
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

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator!=(
        binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator const &other) const
{
    return !(*this == other);
}

template<
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
        if (!_way.empty())
        {
            if (_way.top()->left_subtree_address == _current_node)
            {
                _current_node = _way.top();
                _way.pop();
            }
            else
            {
                while (!_way.empty() && _way.top()->right_subtree_address == _current_node)
                {
                    _current_node = _way.top();
                    _way.pop();
                }

                if (_way.empty())
                {
                    _current_node = nullptr;
                    return *this;
                }

                _current_node = _way.top();
                _way.pop();
            }
        }
        else
        {
            _current_node = nullptr;
        }
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator++(
        int not_used)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator prev_state = *this;

    ++(*this);

    return prev_state;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
std::tuple<unsigned int, tkey const&, tvalue const&> binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator*() const
{
    return std::tuple<unsigned int, tkey const&, tvalue const&>(_way.size(), _current_node->key_and_value.key, _current_node->key_and_value.value);
}

//endregion infix_iterator implementation

//region postfix_iterator implementation

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::postfix_iterator(
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree_root)
{
    _current_node = tree_root;
    while (_current_node != nullptr && _current_node->left_subtree_address != nullptr)
    {
        _way.push(_current_node);
        _current_node = _current_node->left_subtree_address;
    }
}

template<
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

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator!=(
        binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator const &other) const
{
    return !(*this == other);
}

template<
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
                    _current_node = _current_node->left_subtree_address ? _current_node->left_subtree_address : _current_node->right_subtree_address;
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

template<
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

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
std::tuple<unsigned int, tkey const&, tvalue const&> binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator*() const
{
    return std::tuple<unsigned int, tkey const&, tvalue const&>(_way.size(), _current_node->key_and_value.key, _current_node->key_and_value.value);
}

// endregion prefix_iterator implementation

// endregion iterators implementation

// region template methods implementation

// region insertion implementation

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::initialize_node(
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *const node_address,
        const tkey &key,
        tvalue &&value) const
{
    call_node_constructor(node_address);
    node_address->key_and_value.key = key;
    node_address->key_and_value.value = std::move(value);
    node_address->left_subtree_address = node_address->right_subtree_address = nullptr;
    initialize_additional_data(node_address);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::call_node_constructor(
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *node_address) const
{
    new (node_address) node;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::initialize_additional_data(
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *node_address) const
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::insert(
        tkey const &key,
        tvalue &&value,
        node *&subtree_root_address)
{
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> path_to_subtree_root_exclusive;

    return insert_inner(key, std::move(value), subtree_root_address, path_to_subtree_root_exclusive);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::insert_inner(
        tkey const &key,
        tvalue &&value,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

    auto insert_node = &subtree_root_address;

    while (*insert_node != nullptr)
    {
        int compare_result = _tree->_comparator(key, (*insert_node)->key_and_value.key);

        if (compare_result == 0)
        {
            std::string message = "Key already exists";
            this->warning_with_guard("[BST] " + message + ".");

            throw std::invalid_argument(message);
        }

        path_to_subtree_root_exclusive.push(insert_node);
        insert_node = &(compare_result > 0 ? (*insert_node)->right_subtree_address : (*insert_node)->left_subtree_address);
    }

    (*insert_node) = reinterpret_cast<node *>(allocate_with_guard(get_node_size()));
    initialize_node((*insert_node), key, std::move(value));

    after_insert_inner(key, *insert_node, path_to_subtree_root_exclusive);

    this->trace_with_guard("Node with key: " + to_string(key) + " inserted");
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::before_insert_inner(
        tkey const &key,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::after_insert_inner(
        tkey const &key,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    this->trace_with_guard("[BST] Node with key: {" + to_string(key) + "} inserted.");
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
size_t binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::get_node_size() const
{
    return sizeof(node);
}



template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
logger *binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::get_logger() const
{
    return _tree->_logger;
}

// endregion insertion implementation

// region reading implementation

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::read(
        typename associative_container<tkey, tvalue>::key_value_pair * target_key_and_result_value,
        node *&tree_root_address)
{
    std::stack<node **> path_to_subtree_root_exclusive;

    return read_inner(target_key_and_result_value, tree_root_address, path_to_subtree_root_exclusive);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::read_inner(
        typename associative_container<tkey, tvalue>::key_value_pair * target_key_and_result_value,
        node *&subtree_root_address,
        std::stack<node **> &path_to_subtree_root_exclusive)
{
    if (subtree_root_address == nullptr)
    {
        std::string message = "Key not found";
        this->warning_with_guard(message);

        throw std::invalid_argument("[BST] " + message + ".");
    }

    tkey_comparer comparator;

    auto find_node = &subtree_root_address;

    while (*find_node != nullptr)
    {
        int compare_result = comparator(target_key_and_result_value->key, (*find_node)->key_and_value.key);

        if (compare_result == 0)
        {
            target_key_and_result_value->value = std::move((*find_node)->key_and_value.value);
            after_read_inner(target_key_and_result_value, *find_node, path_to_subtree_root_exclusive);
            return true;
        }
        else
        {
            path_to_subtree_root_exclusive.push(find_node);
            find_node = &(compare_result > 0 ? (*find_node)->right_subtree_address : (*find_node)->left_subtree_address);
        }
    }
    return false;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::before_read_inner(
        typename associative_container<tkey, tvalue>::key_value_pair * target_key_and_result_value,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::after_read_inner(
        typename associative_container<tkey, tvalue>::key_value_pair * target_key_and_result_value,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
logger *binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::get_logger() const
{
    return _tree->_logger;
}

// endregion reading implementation

// region removing implementation

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
tvalue binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::remove(
        tkey const &key,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&tree_root_address)
{
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> path_to_subtree_root_exclusive;

    return remove_inner(key, tree_root_address, path_to_subtree_root_exclusive);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
tvalue binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::remove_inner(
        tkey const &key,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    if (subtree_root_address == nullptr)
    {
        std::string message = "Tree is empty";
        this->warning_with_guard(message);

        throw std::invalid_argument("[BST] " + message + ".");
    }

    auto current_node = &subtree_root_address;
    node ** removed_node = nullptr;

    while (*current_node != nullptr)
    {
        int compare_result = _tree->_comparator(key, (*current_node)->key_and_value.key);

        if (compare_result == 0)
        {
            removed_node = current_node;
            break;
        }
        else
        {
            path_to_subtree_root_exclusive.push(current_node);
            current_node = &(compare_result > 0 ? (*current_node)->right_subtree_address : (*current_node)->left_subtree_address);
        }
    }

    if (removed_node == nullptr)
    {
        std::string message = "Key not found";
        this->warning_with_guard(message);

        throw std::invalid_argument("[BST] " + message + ".");
    }

    tvalue removed_value = (*removed_node)->key_and_value.value;

    auto removal_completed = false;

    node * node_for_info = nullptr; //for red_black_tree

    if ((*removed_node)->left_subtree_address != nullptr && (*removed_node)->right_subtree_address != nullptr)
    {
        //auto successor = (*removed_node)->right_subtree_address;
        auto successor = (*removed_node)->left_subtree_address;

        path_to_subtree_root_exclusive.push(removed_node);
        //path_to_subtree_root_exclusive.push(&(*removed_node)->right_subtree_address);
        path_to_subtree_root_exclusive.push(&(*removed_node)->left_subtree_address);

        while (successor->right_subtree_address != nullptr)
        {
            //path_to_subtree_root_exclusive.push(&(successor->left_subtree_address));
            path_to_subtree_root_exclusive.push(&(successor->right_subtree_address));
            //successor = successor->left_subtree_address;
            successor = successor->right_subtree_address;
        }

        auto node = &(*removed_node);

        auto tmp_value = std::move((*node)->key_and_value.value);
        auto tmp_key = std::move((*node)->key_and_value.key);

        (*node)->key_and_value.key = std::move(successor->key_and_value.key);
        (*node)->key_and_value.value = std::move(successor->key_and_value.value);

        successor->key_and_value.key = std::move(tmp_key);
        successor->key_and_value.value = std::move(tmp_value);

        removed_node = path_to_subtree_root_exclusive.top();
        path_to_subtree_root_exclusive.pop();
    }

//    if ((*removed_node)->left_subtree_address != nullptr && (*removed_node)->right_subtree_address != nullptr)
//    {
//        auto successor = (*removed_node)->right_subtree_address;
//
//        if (successor->left_subtree_address == nullptr)
//        {
//            successor->left_subtree_address = (*removed_node)->left_subtree_address;
//            (*removed_node)->~node();
//            deallocate_with_guard(*removed_node);
//            *removed_node = successor;
//            removal_completed = true;
//        }
//        else
//        {
//            path_to_subtree_root_exclusive.push(removed_node);
//            path_to_subtree_root_exclusive.push(&(*removed_node)->right_subtree_address);
//
//            while (successor->left_subtree_address != nullptr)
//            {
//                path_to_subtree_root_exclusive.push(&(successor->left_subtree_address));
//                successor = successor->left_subtree_address;
//            }
//
//            auto node = &(*removed_node);
//
//            (*node)->key_and_value._key = successor->key_and_value._key;
//            (*node)->key_and_value._value = std::move(successor->key_and_value._value);
//
//            removed_node = path_to_subtree_root_exclusive.top();
//            path_to_subtree_root_exclusive.pop();
//        }
//    }

    if (!removal_completed)
    {
        node_for_info = *removed_node;

        before_remove_inner(key, node_for_info, path_to_subtree_root_exclusive);

        if ((*removed_node)->left_subtree_address == nullptr && (*removed_node)->right_subtree_address == nullptr)
        {
            (*removed_node)->~node();
            deallocate_with_guard((*removed_node));
            (*removed_node) = nullptr;
        }
        else
        {
            auto tmp = (*removed_node)->left_subtree_address != nullptr ? (*removed_node)->left_subtree_address : (*removed_node)->right_subtree_address;
            swap_additional_nodes_data(*removed_node, tmp);
            (*removed_node)->~node();
            deallocate_with_guard((*removed_node));
            (*removed_node) = tmp;
        }
    }

    after_remove_inner(key, node_for_info, path_to_subtree_root_exclusive);

    this->trace_with_guard("Node with key: " + to_string(key) + " removed");

    return removed_value;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::before_remove_inner(
        tkey const &key,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::after_remove_inner(
        tkey const &key,
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    this->trace_with_guard("[BST] Node with key: {" + to_string(key) + "} has been deleted.");
}



template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
logger *binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::get_logger() const
{
    return _tree->_logger;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::swap_additional_nodes_data(
        binary_search_tree::node *one_node, binary_search_tree::node *another_node)
{

}

// endregion implementation

// endregion template methods

// region construction, assignment, destruction implementation

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::insertion_template_method(
        binary_search_tree<tkey, tvalue, tkey_comparer> *tree):
        _tree(tree)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::reading_template_method(
        binary_search_tree<tkey, tvalue, tkey_comparer> *tree):
        _tree(tree)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::removing_template_method(
        binary_search_tree<tkey, tvalue, tkey_comparer> *tree):
        _tree(tree)
{

}

template<
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
    _insertion->_tree = this;
    other._insertion = nullptr;

    _reading = other._reading;
    _reading->_tree = this;
    other._reading = nullptr;

    _removing = other._removing;
    _removing->_tree = this;
    other._removing = nullptr;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *binary_search_tree<tkey, tvalue, tkey_comparer>::copy() const
{
    return copy_inner(_root);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *binary_search_tree<tkey, tvalue, tkey_comparer>::copy_inner(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *to_copy) const
{
    if (to_copy == nullptr)
    {
        return nullptr;
    }

    auto *node_copy = reinterpret_cast<binary_search_tree<tkey, tvalue, tkey_comparer>::node *>(allocate_with_guard(get_node_size()));

    new (node_copy) binary_search_tree<tkey, tvalue, tkey_comparer>::node;

    node_copy->key_and_value.key = to_copy->key_and_value.key;
    node_copy->key_and_value.value = to_copy->key_and_value.value;
    node_copy->left_subtree_address = copy_inner(to_copy->left_subtree_address);
    node_copy->right_subtree_address = copy_inner(to_copy->right_subtree_address);

    return node_copy;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::clear()
{
    clear_inner(_root);
}

template<
        typename tkey, typename tvalue, typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::clear_inner(binary_search_tree::node *to_clear)
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

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
        binary_search_tree::insertion_template_method *insertion,
        binary_search_tree::reading_template_method *reading,
        binary_search_tree::removing_template_method *removing,
        memory *allocator,
        logger *logger):
        _comparator(),
        _root(nullptr),
        _insertion(insertion),
        _reading(reading),
        _removing(removing),
        _allocator(allocator),
        _logger(logger)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
        memory *allocator,
        logger *logger):
        _insertion(new insertion_template_method(this)),
        _reading(new reading_template_method(this)),
        _removing(new removing_template_method(this)),
        _allocator(allocator),
        _logger(logger),
        _root(nullptr)
{
    this->trace_with_guard("The tree has been created.");
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
        const binary_search_tree<tkey, tvalue, tkey_comparer> &other)
        : binary_search_tree<tkey, tvalue, tkey_comparer>(other._allocator, other._logger)
{
    _root = other.copy();

    this->trace_with_guard("The tree has been copied.");
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
        binary_search_tree<tkey, tvalue, tkey_comparer> &&other) noexcept
{
    move(std::move(other));

    this->trace_with_guard("The tree has been moved.");
}

template<
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
    _root = other.copy();

    return *this;
}

template<
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

template<
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

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insert(tkey const &key,tvalue &&value)
{
    return _insertion->insert(key, std::move(value), _root);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::find(typename associative_container<tkey, tvalue>::key_value_pair * target_key_and_result_value)
{
    return _reading->read(target_key_and_result_value, _root);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
tvalue binary_search_tree<tkey, tvalue, tkey_comparer>::remove(tkey const &key)
{
    return _removing->remove(key, _root);
}

// endregion associative_container contract implementation

// region logger_holder, memory_holder and other contract implementation

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
logger *binary_search_tree<tkey, tvalue, tkey_comparer>::get_logger() const
{
    return _logger;
}



template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
size_t binary_search_tree<tkey, tvalue, tkey_comparer>::get_node_size() const
{
    return sizeof(node);
}

// endregion logger_holder, memory_holder and other contract implementation

// region iterators requesting implementation

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::begin_prefix() const noexcept
{
    return prefix_iterator(_root);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::end_prefix() const noexcept
{
    return prefix_iterator(nullptr);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::begin_infix() const noexcept
{
    return infix_iterator(_root);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::end_infix() const noexcept
{
    return infix_iterator(nullptr);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::begin_postfix() const noexcept
{
    return postfix_iterator(_root);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::end_postfix() const noexcept
{
    return postfix_iterator(nullptr);
}

// endregion iterators requesting implementation

#endif //BINARY_SEARCH_TREE_H
#endif //MAIN_CPP_BST_H
