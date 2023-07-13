#ifndef MAIN_CPP_SPLAY_TREE_H
#define MAIN_CPP_SPLAY_TREE_H
#include "./binary_search_tree/binary_search_tree_2.h"

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
class splay_tree final : public binary_search_tree<tkey, tvalue, tkey_comparer>
{

protected:
    class splay_insertion_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method
    {
    private:
        splay_tree<tkey, tvalue, tkey_comparer> *_tree;

    protected:
        void after_insert_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

        void insert_inner(
                tkey const &key,
                tvalue &&value,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

    private:
        size_t get_node_size() const override;

        // void call_constructor_node(
        //     typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *mem) const override;

        // void initialize_node_additional_data(
        //     typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *mem) const override;

        // void inject_additional_data(
        //     typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *from,
        //     typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *to) const override;

    public:
        explicit splay_insertion_template_method(
                splay_tree<tkey, tvalue, tkey_comparer> *tree);
    };

protected:
    class splay_removing_template_method final : public binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method
    {
    private:
        splay_tree<tkey, tvalue, tkey_comparer> *_tree;

    protected:
        void after_remove_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

        tvalue remove_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
                std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

    public:
        explicit splay_removing_template_method(
                splay_tree<tkey, tvalue, tkey_comparer> *tree);
    };

protected:
    void splay(
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
            std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive);

    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *merge(
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&tree_first,
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&tree_second);

public:
    explicit splay_tree(memory *allocator = nullptr,
                        logger *logger = nullptr);

    ~splay_tree() final = default;

public:
    splay_tree(
            splay_tree<tkey, tvalue, tkey_comparer> const &other);

    splay_tree(
            splay_tree<tkey, tvalue, tkey_comparer> &&other) noexcept;

    splay_tree &operator=(
            splay_tree<tkey, tvalue, tkey_comparer> const &other);

    splay_tree &operator=(
            splay_tree<tkey, tvalue, tkey_comparer> &&other) noexcept;
};

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void splay_tree<tkey, tvalue, tkey_comparer>::
splay(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
      std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *current_node = subtree_root_address;
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **parent = nullptr;
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **grand_parent = nullptr;

    while (!path_to_subtree_root_exclusive.empty())
    {
        parent = path_to_subtree_root_exclusive.top();
        path_to_subtree_root_exclusive.pop();

        if (path_to_subtree_root_exclusive.size() > 1)
        {
            grand_parent = path_to_subtree_root_exclusive.top();
            path_to_subtree_root_exclusive.pop();
        }
        if ((*parent)->left_subtree_address == current_node)
        {
            if (grand_parent == nullptr)
            {
                this->right_rotation(parent);
            }
            else if (*parent == (*grand_parent)->left_subtree_address)
            {
                this->right_rotation(grand_parent);
                this->right_rotation(parent);
                // current_node = *grand_parent;
            }
            else
            {
                this->right_rotation(parent);
                this->left_rotation(parent);
                // current_node = *parent;
            }
        }
        else
        {
            if (grand_parent == nullptr)
            {
                this->left_rotation(parent);
            }
            else if (*parent == (*grand_parent)->right_subtree_address)
            {
                this->left_rotation(grand_parent);
                this->left_rotation(parent);
                // current_node = *grand_parent;
            }
            else
            {
                this->left_rotation(parent);
                this->right_rotation(parent);
                // current_node = *parent;
            }
        }
    }
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *splay_tree<tkey, tvalue, tkey_comparer>::merge(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&tree_first,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&tree_second)
{
    if (tree_first == nullptr && tree_second != nullptr)
    {
        return tree_second;
    }
    else if (tree_first != nullptr && tree_second == nullptr)
    {
        return tree_first;
    }
    else
    {
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> path;
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *max_node = tree_first;
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **parent = &tree_first;

        while (max_node->right_subtree_address != nullptr)
        {
            path.push(parent);
            parent = &(max_node->right_subtree_address);
            max_node = max_node->right_subtree_address;
        }

        splay(max_node, path);

        tree_first->right_subtree_address = tree_second;

        return tree_first;
    }

    return nullptr;
}

// begin region insertion template method

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void splay_tree<tkey, tvalue, tkey_comparer>::splay_insertion_template_method::after_insert_inner(
        tkey const &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    _tree->splay(subtree_root_address, path_to_subtree_root_exclusive);

    this->trace_with_guard(" Node with key: {" + std::to_string(key) + "} inserted.");
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void splay_tree<tkey, tvalue, tkey_comparer>::splay_insertion_template_method::insert_inner(
        tkey const &key,
        tvalue &&value,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    if (subtree_root_address == nullptr)
    {
        subtree_root_address = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *>(this->allocate_with_guard(get_node_size()));

        // this->trace_with_guard("Allocated root");

        this->initialize_node(subtree_root_address, key, std::move(value));

        after_insert_inner(key, subtree_root_address, path_to_subtree_root_exclusive);
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
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
size_t splay_tree<tkey, tvalue, tkey_comparer>::splay_insertion_template_method::get_node_size() const
{
    return sizeof(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node);
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
splay_tree<tkey, tvalue, tkey_comparer>::splay_insertion_template_method::splay_insertion_template_method(
        splay_tree<tkey, tvalue, tkey_comparer> *tree) : _tree(tree),
                                                         binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method(tree)
{
}

// end region insertion template method

// begin region removing template method

template <typename tkey,
        typename tvalue,
        typename tkey_comparer>
tvalue splay_tree<tkey, tvalue, tkey_comparer>::splay_removing_template_method::remove_inner(
        tkey const &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    if (subtree_root_address == nullptr)
    {
        throw std::invalid_argument("Invalid key to remove");
    }

    std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> path;
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **current_node = &subtree_root_address;
    tvalue remove_value;

    while (*current_node != nullptr)
    {

        if (_tree->_comparator(key, (*current_node)->key) == 0)
        {
            remove_value = (*current_node)->value;

            _tree->splay(*current_node, path);

            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *left_subtree = subtree_root_address->left_subtree_address;
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *right_subtree = subtree_root_address->right_subtree_address;

            subtree_root_address->~node();
            _tree->deallocate_with_guard(subtree_root_address);

            this->trace_with_guard("Node with key: {" + std::to_string(key) + "} has been deleted.");

            _tree->_root = _tree->merge(left_subtree, right_subtree);
            break;
        }
        else
        {
            path.push(current_node);
            current_node = &(_tree->_comparator(key, (*current_node)->key) > 0 ? (*current_node)->right_subtree_address : (*current_node)->left_subtree_address);
        }
    }
    return remove_value;
}

template <typename tkey,
        typename tvalue,
        typename tkey_comparer>
void splay_tree<tkey, tvalue, tkey_comparer>::splay_removing_template_method::after_remove_inner(
        tkey const &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
        std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
}

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
splay_tree<tkey, tvalue, tkey_comparer>::splay_removing_template_method::splay_removing_template_method(
        splay_tree<tkey, tvalue, tkey_comparer> *tree)
        : _tree(tree),
          binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method(tree)
{
}
// end region removing template method

// begin region constructor

template <
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
splay_tree<tkey, tvalue, tkey_comparer>::splay_tree(memory *allocator,
                                                    logger *logger)
        : binary_search_tree<tkey, tvalue, tkey_comparer>(
        new splay_insertion_template_method(this),
        new typename binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method(this),
        new splay_removing_template_method(this),
        allocator,
        logger)
{
}

// end region constructor
#endif //MAIN_CPP_SPLAY_TREE_H
