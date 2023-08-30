#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "bst.h"

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
class avl_tree final : public binary_search_tree<tkey, tvalue, tkey_comparer>
{

private:

    struct avl_node final : public binary_search_tree<tkey, tvalue, tkey_comparer>::node
    {
        int height = 1;
    };

private:

    class avl_tree_insertion_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method
    {

    protected:

        void after_insert_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

        void call_node_constructor(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * node_address) const override;

        size_t get_node_size() const override;

    public:

        explicit avl_tree_insertion_template_method(avl_tree<tkey, tvalue, tkey_comparer> * tree);

    };

    class avl_tree_removing_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method
    {

    protected:

        void after_remove_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

    public:

        explicit avl_tree_removing_template_method(avl_tree<tkey, tvalue, tkey_comparer> * tree);

    };

private:

    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *copy_inner(
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *to_copy) const override;

    size_t get_node_size() const override;

private:

    int height(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * subtree_root);

    void fix_height(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *& subtree_root);

    int balance_factor(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node * subtree_root);

public:

    explicit avl_tree(memory *allocator = nullptr, logger *logger = nullptr);

    ~avl_tree() final = default;

    avl_tree(
            avl_tree const &other);

    avl_tree(
            avl_tree &&other) noexcept;

    avl_tree &operator=(
            avl_tree const &other);

    avl_tree &operator=(
            avl_tree &&other) noexcept;

};

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
int avl_tree<tkey, tvalue, tkey_comparer>::height(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *subtree_root)
{
    return subtree_root != nullptr ? reinterpret_cast<avl_node*>(subtree_root)->height : 0;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::fix_height(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root)
{
    int left_height = subtree_root == nullptr ? 0 : height(subtree_root->left_subtree_address);
    int right_height = subtree_root == nullptr ? 0 : height(subtree_root->right_subtree_address);

    if (subtree_root != nullptr)
    {
        reinterpret_cast<avl_node*>(subtree_root)->height = (left_height > right_height ? left_height : right_height) + 1;
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
int avl_tree<tkey, tvalue, tkey_comparer>::balance_factor(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *subtree_root)
{
    return height(subtree_root->left_subtree_address) - height(subtree_root->right_subtree_address);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_insertion_template_method::after_insert_inner(
        const tkey &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node ** current_node = nullptr;

    while (!path_to_subtree_root_exclusive.empty())
    {
        current_node = path_to_subtree_root_exclusive.top();
        path_to_subtree_root_exclusive.pop();

        reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height(*current_node);

        int balance_factor = reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->balance_factor(*current_node);

        if (balance_factor == 2)
        {
            if (reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->balance_factor((*current_node)->left_subtree_address) == -1)
            {
                reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->left_rotation((*current_node)->left_subtree_address);
                reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height((*current_node)->left_subtree_address->left_subtree_address);
                reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height((*current_node)->left_subtree_address);
            }

            reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->right_rotation(*current_node);
            reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height((*current_node)->right_subtree_address);
            reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height(*current_node);
        }

        if (balance_factor == -2)
        {
            if (reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->balance_factor((*current_node)->right_subtree_address) == 1)
            {
                reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->right_rotation((*current_node)->right_subtree_address);
                reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height((*current_node)->right_subtree_address->right_subtree_address);
                reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height((*current_node)->right_subtree_address);
            }

            reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->left_rotation(*current_node);
            reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height((*current_node)->left_subtree_address);
            reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height(*current_node);
        }
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_insertion_template_method::call_node_constructor(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *node_address) const
{
    new (node_address) avl_node;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
size_t avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_insertion_template_method::get_node_size() const
{
    return sizeof(avl_node);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_removing_template_method::after_remove_inner(
        const tkey &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node ** current_node = nullptr;

    while (!path_to_subtree_root_exclusive.empty())
    {
        current_node = path_to_subtree_root_exclusive.top();
        path_to_subtree_root_exclusive.pop();

        reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height(*current_node);

        int balance_factor = reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->balance_factor(*current_node);

        if (balance_factor == 2)
        {
            if (reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->balance_factor((*current_node)->left_subtree_address) == -1)
            {
                reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->left_rotation((*current_node)->left_subtree_address);
                reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height((*current_node)->left_subtree_address->left_subtree_address);
                reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height((*current_node)->left_subtree_address);
            }

            reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->right_rotation(*current_node);
            reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height((*current_node)->right_subtree_address);
            reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height(*current_node);
        }

        if (balance_factor == -2)
        {
            if (reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->balance_factor((*current_node)->right_subtree_address) == 1)
            {
                reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->right_rotation((*current_node)->right_subtree_address);
                reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height((*current_node)->right_subtree_address->right_subtree_address);
                reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height((*current_node)->right_subtree_address);
            }

            reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->left_rotation(*current_node);
            reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height((*current_node)->left_subtree_address);
            reinterpret_cast<avl_tree<tkey, tvalue, tkey_comparer> *>(this->_tree)->fix_height(*current_node);
        }
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_insertion_template_method::avl_tree_insertion_template_method(
        avl_tree<tkey, tvalue, tkey_comparer> *tree)
        : binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method(tree)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree_removing_template_method::avl_tree_removing_template_method(
        avl_tree<tkey, tvalue, tkey_comparer> *tree)
        : binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method(tree)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree(memory *allocator, logger *logger):
        binary_search_tree<tkey, tvalue, tkey_comparer>(
                new avl_tree_insertion_template_method(this),
                new typename binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method(this),
                new avl_tree_removing_template_method(this),
                allocator,
                logger)
{
    this->trace_with_guard("[AVL TREE] The tree has been created.");
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
size_t avl_tree<tkey, tvalue, tkey_comparer>::get_node_size() const
{
    return sizeof(avl_node);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* avl_tree<tkey, tvalue, tkey_comparer>::copy_inner(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *to_copy) const
{
    if (to_copy == nullptr)
    {
        return nullptr;
    }

    auto *node_copy = reinterpret_cast<avl_node*>(this->allocate_with_guard(get_node_size()));

    new (node_copy) avl_node;
    node_copy->key_and_value.key = to_copy->key_and_value.key;
    node_copy->key_and_value.value = to_copy->key_and_value.value;
    node_copy->height = reinterpret_cast<avl_node*>(to_copy)->height;
    node_copy->left_subtree_address = copy_inner(to_copy->left_subtree_address);
    node_copy->right_subtree_address = copy_inner(to_copy->right_subtree_address);

    return node_copy;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree(avl_tree<tkey, tvalue, tkey_comparer> const &other)
        : avl_tree<tkey, tvalue, tkey_comparer>(other.get_outer_allocator(), other.get_logger())
{
    this->_root = other.copy();

    this->trace_with_guard("The tree has been copy.");
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree(avl_tree<tkey, tvalue, tkey_comparer> &&other) noexcept
        : binary_search_tree<tkey, tvalue, tkey_comparer>(std::move(other))
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer> &avl_tree<tkey, tvalue, tkey_comparer>::operator=(avl_tree<tkey, tvalue, tkey_comparer> const &other)
{
    static_cast<binary_search_tree<tkey, tvalue, tkey_comparer> &>(*this) = other;

    return *this;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer> &avl_tree<tkey, tvalue, tkey_comparer>::operator=(avl_tree<tkey, tvalue, tkey_comparer> &&other) noexcept
{
    static_cast<binary_search_tree<tkey, tvalue, tkey_comparer> &>(this) = std::move(other);

    return *this;
}

#endif //AVL_TREE_H
