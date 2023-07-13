#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "./binary_search_tree/binary_search_tree_2.h"

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
class avl_tree final : public binary_search_tree<tkey, tvalue, tkey_comparer>
{

protected:
    struct avl_node : binary_search_tree<tkey, tvalue, tkey_comparer>::node
    {
        size_t height = 0;
    };

protected:
    class avl_insertion_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method
    {
    private:
        avl_tree<tkey, tvalue, tkey_comparer> *_tree;

    protected:
        void after_insert_inner(
            tkey const &key,
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
            std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

    private:
        size_t get_node_size() const override;

        void call_constructor_node(
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *mem) const override;

        void initialize_node_additional_data(
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *mem) const override;

        void inject_additional_data(
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *from,
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *to) const override;

    public:
        explicit avl_insertion_template_method(
            avl_tree<tkey, tvalue, tkey_comparer> *tree);
    };

protected:
    class avl_removing_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method
    {
    private:
        avl_tree<tkey, tvalue, tkey_comparer> *_tree;

    protected:
        void after_remove_inner(
            tkey const &key,
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
            std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

    public:
        explicit avl_removing_template_method(
            avl_tree<tkey, tvalue, tkey_comparer> *tree);
    };

public:
    explicit avl_tree(memory *allocator = nullptr,
                      logger *logger = nullptr);

    ~avl_tree() final = default;

private:
    int get_height(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *node);

    void fix_height(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **subtree_address);

    size_t balance_factor(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *subtree_address);

public:
    avl_tree(
        avl_tree<tkey, tvalue, tkey_comparer> const &other);

    avl_tree(
        avl_tree<tkey, tvalue, tkey_comparer> &&other) noexcept;

    avl_tree &operator=(
        avl_tree<tkey, tvalue, tkey_comparer> const &other);

    avl_tree &operator=(
        avl_tree<tkey, tvalue, tkey_comparer> &&other) noexcept;
};

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
int avl_tree<tkey, tvalue, tkey_comparer>::get_height(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *node)
{
    return node != nullptr ? reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer>::avl_node *>(node)->height : 0;
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
size_t avl_tree<tkey, tvalue, tkey_comparer>::balance_factor(
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *subtree_address)
{
    size_t height_left = get_height(subtree_address->left_subtree_address);
    size_t height_right = get_height(subtree_address->right_subtree_address);

    return height_left - height_right;
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::fix_height(
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **subtree_address)
{
    size_t height_left = *subtree_address != nullptr ? get_height((*subtree_address)->left_subtree_address) : 0;
    size_t height_right = *subtree_address != nullptr ? get_height((*subtree_address)->right_subtree_address) : 0;

    if (*subtree_address != nullptr)
        reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer>::avl_node *>(*subtree_address)->height = (height_left > height_right ? height_left : height_right) + 1;
}

// begin after insert inner

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_insertion_template_method::call_constructor_node(
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *mem) const
{
    new (mem) typename avl_tree<tkey, tvalue, tkey_comparer>::avl_node;
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_insertion_template_method::initialize_node_additional_data(
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *mem) const
{
    reinterpret_cast<typename avl_tree<tkey, tvalue, tkey_comparer>::avl_node *>(mem)->height = 0;
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_insertion_template_method::inject_additional_data(
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *from,
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *to) const
{
    reinterpret_cast<typename avl_tree<tkey, tvalue, tkey_comparer>::avl_node *>(to)->height = reinterpret_cast<typename avl_tree<tkey, tvalue, tkey_comparer>::avl_node *>(from)->height;
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
size_t avl_tree<tkey, tvalue, tkey_comparer>::avl_insertion_template_method::get_node_size() const
{
    return sizeof(typename avl_tree<tkey, tvalue, tkey_comparer>::avl_node);
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::
    avl_insertion_template_method::
        after_insert_inner(tkey const &key,
                           typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                           std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    _tree->fix_height(&subtree_root_address);
    size_t res_bal_factor = _tree->balance_factor(subtree_root_address);

    if (res_bal_factor == 2)
    {
        if (_tree->balance_factor(subtree_root_address->left_subtree_address) == -1)
        {
            _tree->left_rotation(&(subtree_root_address->left_subtree_address));
            _tree->fix_height(&(subtree_root_address->left_subtree_address));
            _tree->fix_height(&(subtree_root_address->left_subtree_address->right_subtree_address));
        }

        _tree->right_rotation(&subtree_root_address);
        _tree->fix_height(&subtree_root_address);
        _tree->fix_height(&(subtree_root_address->left_subtree_address));
    }

    if (res_bal_factor == -2)
    {
        if (_tree->balance_factor(subtree_root_address->right_subtree_address) == 1)
        {
            _tree->right_rotation(&(subtree_root_address->right_subtree_address));
            _tree->fix_height(&(subtree_root_address->right_subtree_address));
            _tree->fix_height(&(subtree_root_address->right_subtree_address->left_subtree_address));
        }
        _tree->left_rotation(&subtree_root_address);
        _tree->fix_height(&subtree_root_address);
        _tree->fix_height(&(subtree_root_address->right_subtree_address));
    }
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_insertion_template_method::avl_insertion_template_method(
    avl_tree<tkey, tvalue, tkey_comparer> *tree) : _tree(tree),
                                                   binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method(tree)
{
}

// end after insert inner

// begin after remove inner
template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_removing_template_method::after_remove_inner(
    tkey const &key,
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

    _tree->fix_height(&subtree_root_address);
    size_t res_bal_factor = _tree->balance_factor(subtree_root_address);

    if (res_bal_factor == 2)
    {
        if (_tree->balance_factor(subtree_root_address->left_subtree_address) == -1)
        {
            _tree->left_rotation(&(subtree_root_address->left_subtree_address));
            _tree->fix_height(&(subtree_root_address->left_subtree_address));
            _tree->fix_height(&(subtree_root_address->left_subtree_address->right_subtree_address));
        }

        _tree->right_rotation(&subtree_root_address);
        _tree->fix_height(&subtree_root_address);
        _tree->fix_height(&(subtree_root_address->left_subtree_address));
    }

    if (res_bal_factor == -2)
    {
        if (_tree->balance_factor(subtree_root_address->right_subtree_address) == 1)
        {
            _tree->right_rotation(&(subtree_root_address->right_subtree_address));
            _tree->fix_height(&(subtree_root_address->right_subtree_address));
            _tree->fix_height(&(subtree_root_address->right_subtree_address->left_subtree_address));
        }
        _tree->left_rotation(&subtree_root_address);
        _tree->fix_height(&subtree_root_address);
        _tree->fix_height(&(subtree_root_address->right_subtree_address));
    }
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_removing_template_method::avl_removing_template_method(
    avl_tree<tkey, tvalue, tkey_comparer> *tree) : _tree(tree),
                                                   binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method(tree)
{
}

// end after remove inner

// begin region constructor

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree(memory *allocator,
                                                logger *logger)
    : binary_search_tree<tkey, tvalue, tkey_comparer>(new avl_insertion_template_method(this),
                                                      new typename binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method(this),
                                                      new avl_removing_template_method(this),
                                                      allocator,
                                                      logger)
{
}

// end region constructor
template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree(avl_tree<tkey, tvalue, tkey_comparer> const &other) : avl_tree<tkey, tvalue, tkey_comparer>(other._allocator, other._logger)
{
    binary_search_tree<tkey, tvalue, tkey_comparer>::_root = other.copy();
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree(
    avl_tree<tkey, tvalue, tkey_comparer> &&other) noexcept
{
    this->move(static_cast<binary_search_tree<tkey, tvalue, tkey_comparer> &&>(std::move(other)));
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer> &avl_tree<tkey, tvalue, tkey_comparer>::operator=(
    avl_tree<tkey, tvalue, tkey_comparer> const &other)
{
    if (this == &other)
    {
        return *this;
    }

    this->clear();

    this->_allocator = other._allocator;
    this->_logger = other._logger;
    this->_root = other.copy();

    return *this;
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer> &avl_tree<tkey, tvalue, tkey_comparer>::operator=(
    avl_tree<tkey, tvalue, tkey_comparer> &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    this->clear();

    delete this->_insertion;
    delete this->_reading;
    delete this->_removing;

    this->move(std::move(other));

    return *this;
}

#endif // AVL_TREE_H