////#include <cmath>
////#include <cstdlib>
////#include <list>
////#include <random>
#include <sstream>
////#include "bigint/bigint.h"
////#include "bigint/bigint_concrete.h"
////#include "bigint/bigint_fraction.h"
////#include "logger/logger.h"
////#include "logger/logger_builder_concrete.h"
////#include "memory/memory.h"
////#include "memory/memory_buddy_system.h"
////#include "memory/memory_with_fit_allocation.h"
////#include "memory/memory_from_global_heap.h"
////#include "memory/memory_with_boundary_tags_deallocation.h"
////#include "memory/memory_with_sorted_list_deallocation.h"
////#include "tree/associative_container.h"
//////#include "tree/avl_tree.h"
//////#include "tree/b_tree.h"
//////#include "tree/binary_search_tree.h"
////#include "tree/bplus_tree.h"
//////#include "tree/red_black_tree.h"
//////#include "tree/splay_tree.h"
////#include "tree/search_tree.h"

/*void logger_demo(
    logger_builder *builder)
{
    logger *constructed_logger = builder
        ->add_file_stream("file1.txt", logger::severity::critical)
        ->add_file_stream("file2.txt", logger::severity::debug)
        ->add_file_stream("file3.txt", logger::severity::trace)
        ->add_console_stream(logger::severity::information)
        ->build();

    logger *constructed_logger_2 = builder
        ->add_file_stream("file4.txt", logger::severity::warning)
        ->add_console_stream(logger::severity::trace)
        ->build();

    constructed_logger
        ->log("kek lol 123", logger::severity::information)
        ->log("kek lol 123", logger::severity::information);

    constructed_logger_2
        ->log("123 kek lol", logger::severity::error);

    delete constructed_logger_2;
    delete constructed_logger;
    delete builder;
}

void memory_global_heap_demo()
{
    logger_builder *builder = new logger_builder_concrete();
    logger *log = builder
        ->add_file_stream("file1.txt", logger::severity::trace)
        ->add_file_stream("file2.txt", logger::severity::debug)
        ->add_file_stream("file3.txt", logger::severity::information)
        ->add_console_stream(logger::severity::warning)
        ->build();

    memory_with_fit_allocation *mem_outer = new memory_with_sorted_list_deallocation(50000, nullptr, log);
    memory_with_fit_allocation *mem_inner = new memory_with_sorted_list_deallocation(15000, mem_outer, log, memory_with_fit_allocation::allocation_mode::the_worst_fit);

    int *ptr = reinterpret_cast<int *>(mem_inner->allocate(sizeof(int) * 30));
    char *ptr1 = reinterpret_cast<char *>(mem_inner->allocate(sizeof(char) * 49));
    int *ptr2 = reinterpret_cast<int *>(const_cast<void *>(*mem_outer += 0));
    int *ptr3 = reinterpret_cast<int *>(const_cast<void *>(*mem_outer += 10));
    auto *ptr4 = reinterpret_cast<unsigned char *>(const_cast<void *>(*mem_outer += 1000));

    srand((unsigned)time(NULL));

    for (auto i = 0; i < 1000; i++)
    {
        *(ptr4 + i) = rand() % (sizeof(unsigned char) << 8);
    }

    *mem_inner -= ptr;
    *mem_inner -= ptr1;
    mem_outer->deallocate(ptr2);
    mem_outer->deallocate(ptr3);
    mem_outer->deallocate(ptr4);

    delete mem_inner;
    delete mem_outer;
    delete log;
    delete builder;
}

class int_comparer final
{

public:

    int operator()(
        int left,
        int right)
    {
        return left - right;
    }

};

void binary_search_tree_demo()
{
    logger_builder *builder = new logger_builder_concrete();
    logger *log = builder
        ->add_file_stream("file1.txt", logger::severity::trace)
        ->add_file_stream("file2.txt", logger::severity::debug)
        ->add_file_stream("file3.txt", logger::severity::information)
        ->add_console_stream(logger::severity::trace)
        ->build();

    memory *mem1 = new memory_with_sorted_list_deallocation(100000, nullptr);
    memory *mem2 = new memory_with_sorted_list_deallocation(50000, mem1);
    memory *mem = new memory_with_sorted_list_deallocation(10000, mem2);

    auto *tree = new obsolete_binary_search_tree<int, std::string, int_comparer>(mem);

    *tree += std::tuple<int, std::string &&>(0, std::move(std::string("1234")));
    *tree += std::tuple<int, std::string &&>(-1, std::move(std::string("2345")));
    *tree += std::tuple<int, std::string &&>(1, std::move(std::string("3456")));
    *tree += std::tuple<int, std::string &&>(-2, std::move(std::string("4567")));
    *tree += std::tuple<int, std::string &&>(2, std::move(std::string("5678")));
    *tree += std::tuple<int, std::string &&>(-3, std::move(std::string("6789")));
    *tree += std::tuple<int, std::string &&>(3, std::move(std::string("7890")));

    auto const &str = tree->get(0);
    *const_cast<std::string *>(&str) = "12345678";

    std::cout << "Prefix iterator:" << std::endl;
    auto end_prefix = tree->end_prefix();
    for (auto it = tree->begin_prefix(); it != end_prefix; ++it)
    {
        for (auto x = 0; x < std::get<0>(*it); x++)
        {
            std::cout << "    ";
        }

        std::cout << "key: " << std::get<1>(*it) << ", value: \"" << std::get<2>(*it) << "\"" << std::endl;
    }

    std::cout << std::endl << "Infix iterator:" << std::endl;
    auto end_infix = tree->end_infix();
    for (auto it = tree->begin_infix(); it != end_infix; ++it)
    {
        for (auto x = 0; x < std::get<0>(*it); x++)
        {
            std::cout << "    ";
        }

        std::cout << "key: " << std::get<1>(*it) << ", value: \"" << std::get<2>(*it) << "\"" << std::endl;
    }

    std::cout << std::endl << "Postfix iterator:" << std::endl;
    auto end_postfix = tree->end_postfix();
    for (auto it = tree->begin_postfix(); it != end_postfix; ++it)
    {
        for (auto x = 0; x < std::get<0>(*it); x++)
        {
            std::cout << "    ";
        }

        std::cout << "key: " << std::get<1>(*it) << ", value: \"" << std::get<2>(*it) << "\"" << std::endl;
    }

    auto removed_value = std::move(*tree -= 0);
    std::cout << removed_value << std::endl;

    for (auto it = tree->begin_prefix(); it != end_prefix; ++it)
    {
        for (auto x = 0; x < std::get<0>(*it); x++)
        {
            std::cout << "    ";
        }

        std::cout << "key: " << std::get<1>(*it) << ", value: \"" << std::get<2>(*it) << "\"" << std::endl;
    }

    delete tree;
    delete mem;
    delete mem2;
    delete mem1;
    delete log;
    delete builder;
}
*/

class int_comparer
{
public:
    int operator()(int x, int y)
    {
        return x - y;
    }
};

/*void bst_test()
{
    logger_builder *allocator_logger_builder = new logger_builder_concrete();
    logger *allocator_logger = allocator_logger_builder
        ->add_file_stream("allocator_tests.txt", logger::severity::trace)
        ->build();
    delete allocator_logger_builder;

    logger_builder *rbtree_logger_builder = new logger_builder_concrete();
    logger *rbtree_logger = rbtree_logger_builder
        ->add_file_stream("rbtree_tests.txt", logger::severity::trace)
        ->build();
    delete rbtree_logger_builder;

    memory *allocator = new memory_from_global_heap(allocator_logger);
    associative_container<int, std::string> *tree = new obsolete_binary_search_tree<int, std::string, int_comparer>(allocator, rbtree_logger);
    srand((unsigned)time(nullptr));

    int i = 1;
    while (1)
    {
        std::cout << "----------------------------------------" << std::endl << "Iteration #" << i << std::endl;

        auto key = rand() % 500;
        switch (rand() % 2)
        {
            case 0:
                try
                {
                    tree->insert(key, "");
                    std::cout << "successfully inserted key \"" << key << "\"" << std::endl;
                }
                catch (obsolete_binary_search_tree<int, std::string, int_comparer>::insertion_exception const &)
                {
                    std::cout << "insertion error - duplicate key \"" << key << "\" detected" << std::endl;
                }
                catch (memory::memory_exception const &)
                {
                    std::cout << "insertion error - allocation failed" << std::endl;
                }
                break;
            case 1:
                try
                {
                    tree->remove(key);
                    std::cout << "successfully removed key \"" << key << "\"" << std::endl;
                }
                catch (obsolete_binary_search_tree<int, std::string, int_comparer>::removing_exception const &)
                {
                    std::cout << "removing error - key \"" << key << "\" not found" << std::endl;
                }
                break;
        }

        if (i++ % 100 == 0)
        {
            std::cout << "Tree state after iteration #" << i << ":" << std::endl;

            auto *bst = reinterpret_cast<obsolete_binary_search_tree<int, std::string, int_comparer> *>(tree);
            auto end_prefix = bst->end_prefix();
            for (auto it = bst->begin_prefix(); it != end_prefix; it++)
            {
                for (auto x = 0; x < std::get<0>(*it); x++)
                {
                    std::cout << "    ";
                }

                std::cout << "key: " << std::get<1>(*it) << ", value: \"" << std::get<2>(*it) << "\"" << std::endl;
            }
        }
    }

    delete tree;
    delete allocator;
    delete rbtree_logger;
    delete allocator_logger;
}*/

void allocator_demo(
    memory *allocator,
    unsigned int iterations_count)
{
    std::list<void *> allocated_blocks;
    std::random_device rd;
    auto mtgen = std::mt19937(rd());
    auto ud = std::uniform_int_distribution<>{0, 2};

    for (auto i = 0; i < iterations_count; i++)
    {
        switch (ud(mtgen))
        {
            case 0:
            case 1:
                try
                {
                    allocated_blocks.push_front(allocator->allocate(rand() % 251 + 50));
                    std::cout << "allocation succeeded" << std::endl;
                }
                catch (memory::memory_exception const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                break;
            case 2:
                if (allocated_blocks.empty())
                {
                    std::cout << "No blocks to deallocate" << std::endl;

                    break;
                }

                auto it = allocated_blocks.begin();
                std::advance(it, rand() % allocated_blocks.size());
                allocator->deallocate(*it);
                allocated_blocks.erase(it);
                std::cout << "deallocation succeeded" << std::endl;
                break;
        }
    }

    while (!allocated_blocks.empty())
    {
        auto it = allocated_blocks.begin();
        std::advance(it, rand() % allocated_blocks.size());
        allocator->deallocate(*it);
        allocated_blocks.erase(it);
        std::cout << "deallocation succeeded" << std::endl;
    }
}

void allocators_demo(
    size_t trusted_memory_size,
    memory_with_fit_allocation::allocation_mode fit_mode,
    unsigned int iterations_count)
{
    logger_builder *builder = new logger_builder_concrete();
    auto *global_heap_allocator_logger = builder
        ->add_file_stream("global heap allocator.txt", logger::severity::debug)
        ->build();
    builder->clear();
    memory *global_heap_allocator = new memory_from_global_heap(global_heap_allocator_logger);

    auto *sorted_list_allocator_logger = builder
        ->add_file_stream("sorted list allocator.txt", logger::severity::trace)
        ->build();
    builder->clear();
    memory *sorted_list_allocator = new memory_with_sorted_list_deallocation(trusted_memory_size, nullptr, sorted_list_allocator_logger, fit_mode);

    auto *boundary_tags_allocator_logger = builder
        ->add_file_stream("boundary tags allocator.txt", logger::severity::debug)
        ->build();
    builder->clear();
    memory *boundary_tags_allocator = new memory_with_boundary_tags_deallocation(trusted_memory_size, nullptr, boundary_tags_allocator_logger, fit_mode);

    auto *buddy_system_allocator_logger = builder
        ->add_file_stream("buddy system allocator.txt", logger::severity::debug)
        ->build();
    delete builder;
    memory *buddy_system_allocator = new memory_buddy_system(static_cast<size_t>(std::log2(trusted_memory_size) + 1), nullptr, buddy_system_allocator_logger, fit_mode);

    allocator_demo(global_heap_allocator, iterations_count);
    delete global_heap_allocator;
    delete global_heap_allocator_logger;

    allocator_demo(sorted_list_allocator, iterations_count);
    delete sorted_list_allocator;
    delete sorted_list_allocator_logger;

    allocator_demo(boundary_tags_allocator, iterations_count);
    delete boundary_tags_allocator;
    delete boundary_tags_allocator_logger;

    allocator_demo(buddy_system_allocator, iterations_count);
    delete buddy_system_allocator;
    delete buddy_system_allocator_logger;
}

void trees_demo()
{

}

class stdstring_comparer final
{
public:
    int operator()(
        std::string const& x,
        std::string const &y)
    {
        auto x_iterator = x.begin();
        auto y_iterator = y.begin();
        auto const x_end_iterator = x.cend();
        auto const y_end_iterator = y.cend();

        while (true)
        {
            auto const diff = *x_iterator - *y_iterator;
            if (diff != 0)
            {
                return diff;
            }

            ++x_iterator;
            ++y_iterator;

            if (x_iterator == x_end_iterator && y_iterator == y_end_iterator)
            {
                return 0;
            }
            else if (x_iterator == x_end_iterator)
            {
                return *y_iterator;
            }
            else if (y_iterator == y_end_iterator)
            {
                return *x_iterator;
            }
        }
    }
};

class string_pool
{
private:
    static string_pool *_instance;
public:
    static string_pool *get_instance()
    {
        if (_instance == nullptr)
        {
            _instance = new string_pool;
        }

        return _instance;
    }

private:

    associative_container<std::string, std::pair<std::string *, unsigned int> > *_pool;

private:

    string_pool()
    {
        _pool = new bplus_tree<std::string, std::pair<std::string *, unsigned int>, stdstring_comparer>(10);
    }

public:

    string_pool(string_pool const &) = delete;

    string_pool(string_pool &&) = delete;

    string_pool &operator=(string_pool const &) = delete;

    string_pool &operator=(string_pool &&) = delete;

    ~string_pool() noexcept
    {
        delete _pool;
    }

public:

    std::string *get_string(std::string const &key)
    {
        //try
        //{
        //    return std::string_pool->get(key);
        //}
        //catch (search_tree<std::string, std::string *, stdstring_comparer>::reading_exception const &ex)
        //{
        //    auto * string_to_insert = new std::string(key);
        //    _pool->insert(key, std::move(string_to_insert));
        //    return _pool->get(key);
        //}
        // TODO: гавнище джяляб пиздэц
        return new std::string("");
    }

    void remove_string(std::string const &key)
    {
        try
        {
            // TODO
        }
        catch (search_tree<std::string, std::string *, stdstring_comparer>::removing_exception const &ex)
        {
            // TODO
        }
    }

};

string_pool *string_pool::_instance = nullptr;
//TODO: это что такое, куда пихать потом инстанс??

template<typename tkey, typename tvalue, typename tkey_comparer>
std::stringstream &print_node(std::stringstream &stream, typename search_tree<tkey, tvalue, tkey_comparer>::search_tree_node *node)
{
    stream << "<";

    if (node->involved_keys == 0)
    {
        stream << " empty node ";
    }

    for (auto i = 0; i <= node->involved_keys; i++)
    {
        stream << '{' << static_cast<char>(reinterpret_cast<long long>(node->subtrees[i]) + 'a') << "}";

        if (i != node->involved_keys)
        {
            stream << " <" << node->keys_and_values[i].key << ", \"" << node->keys_and_values[i].value << "\"> ";
        }
    }

    stream << ">";
    return stream;
}

void test_search_tree()
{
    //std::srand(12345);
    //search_tree<int, std::string, int_comparer> puki4;
    //auto shell = puki4._context->create_node_shell(2);
    //shell->subtrees[0] = reinterpret_cast<typename search_tree<int, std::string, int_comparer>::search_tree_node *>(1);
    //auto i = 0;
    //while (true)
    //{
    //    auto index = rand() % (shell->involved_keys + 1);
    //    auto splitted_node = puki4._context
    //        ->shift(shell, index, search_tree<int, std::string, int_comparer>::node_interaction_context::node_split_mode::do_not_copy_median_kvp, i, std::string(""), reinterpret_cast<typename search_tree<int, std::string, int_comparer>::search_tree_node *>(i * 2 + 1), reinterpret_cast<typename search_tree<int, std::string, int_comparer>::search_tree_node *>(i * 2 + 2));
    //    i++;
//
    //    if (splitted_node.has_value())
    //    {
    //        std::stringstream str;
    //        print_node<int, std::string, int_comparer>(str, shell);
    //        std::cout << "After adding to position " << index << " and split: " << str.str() << ", ";
//
    //        str = std::move(std::stringstream{});
    //        print_node<int, std::string, int_comparer>(str, splitted_node.value().first);
    //        std::cout << str.str();
//
    //        break;
    //    }
//
    //    std::stringstream str;
    //    print_node<int, std::string, int_comparer>(str, shell);
    //    std::cout << "After adding to position " << index << ": " << str.str() << std::endl;
    //}
}

void test_flyweight_pool()
{
    // auto * ptr = string_pool::get_instance()->get_string("test");
    // std::cout << ptr << ": " << *ptr << std::endl;
    // auto *ptr2 = string_pool::get_instance()->get_string("test");
    // std::cout << ptr2 << ": " << *ptr2 << std::endl;
}

void bplus_tree_test()
{
    srand(12345);
    logger_builder *builder = new logger_builder_concrete();
    auto *sorted_list_allocator_logger = builder
        ->add_file_stream("sorted list allocator.txt", logger::severity::debug)
        ->build();
    delete builder;
    memory *sorted_list_allocator = new memory_with_sorted_list_deallocation(2000000, nullptr, sorted_list_allocator_logger, memory_with_fit_allocation::allocation_mode::the_worst_fit);

    bplus_tree<int, std::string, int_comparer> denis(3, sorted_list_allocator);

    auto i = 0;
    auto const values_count = 1000;
    std::vector<int> v(values_count + 1);
    for (i = 0; i <= values_count; i++)
    {
        v[i] = i;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(v.begin(), v.end(), g);

    for (i = 0; i < values_count; i++)
    {
        try
        {
            denis.insert(v[i], std::move(std::to_string(v[i] * 2 + 1)));
            //std::cout << "State after iteration #" << i << " == " << std::endl;
            //denis.iterate_terminal_nodes_list();
            //
            //std::cout << std::endl;
        }
        catch (search_tree<int, std::string, int_comparer>::insertion_exception const &ex)
        {

        }
        catch (memory::memory_exception const &ex)
        {
            std::cout << "allocator puk puk((9(9" << std::endl;
        }
    }

    std::cout << "State after iteration #" << i << ':' << std::endl;
    denis.iterate_tree();

    std::cout << std::endl << std::endl << "Terminal nodes list after iteration #" << i << ':' << std::endl;
    denis.iterate_terminal_nodes_list();

    std::cout << '\"' << denis.get(123) << '\"';

    delete sorted_list_allocator;
    delete sorted_list_allocator_logger;
}

class logger_singleton : public logger_holder
{
private:
    static logger_singleton *_instance;
public:
    static logger_singleton *get_instance()
    {
        if (_instance == nullptr)
        {
            _instance = new logger_singleton("logger_settings.json");
        }

        return _instance;
    }
private:
    logger *_logger;
private:
    logger_singleton(std::string const &logger_config_file_path)
    {
        // TODO: use json

        auto *builder = new logger_builder_concrete;
        builder
            ->add_file_stream("logs.txt", logger::severity::trace);
        _logger = builder->build();
        delete builder;
    }
public:
    ~logger_singleton() { delete _logger; }
    logger_singleton(logger_singleton const &) = delete;
    logger_singleton(logger_singleton &&) = delete;
    logger_singleton &operator=(logger_singleton const &) = delete;
    logger_singleton &operator=(logger_singleton &&) = delete;
public:
    logger *get_logger() const noexcept override
    {
        return _logger;
    }
};

logger_singleton *logger_singleton::_instance = nullptr;

////TODO: это что такое, куда пихать потом инстанс??

enum class allocator_type
{
    global_heap,
    sorted_list,
    boundary_tags,
    buddy_system
};

template<typename trequest> class command
{
public:
    virtual ~command() = default;
public:
    virtual bool can_execute(trequest const &) const noexcept = 0;
    virtual void execute(trequest const &) const noexcept = 0;
};

template<typename trequest> class request_handler_with_command_chain;

template<typename trequest> class abstract_handler
{
    friend class request_handler_with_command_chain<trequest>;
protected:
    abstract_handler<trequest> *_next_handler;
public:
    abstract_handler()
        : _next_handler(nullptr)
    {

    }
    virtual ~abstract_handler() = default;
public:
    virtual bool handle(trequest const &) const noexcept = 0;
};

template<typename trequest> class request_handler_with_command : public abstract_handler<trequest>
{
private:
    command<trequest> *_target_action;
public:
    request_handler_with_command(command<trequest> *target_action)
        : _target_action(target_action)
    {

    }

    ~request_handler_with_command()
    {
        delete _target_action;
    }

    request_handler_with_command(request_handler_with_command const &) = delete;
    request_handler_with_command(request_handler_with_command &&) = delete;
    request_handler_with_command &operator=(request_handler_with_command const &) = delete;
    request_handler_with_command &operator=(request_handler_with_command &&) = delete;
public:
    bool handle(trequest const &request) const noexcept final
    {
        if (!_target_action->can_execute(request))
        {
            if (abstract_handler<trequest>::_next_handler == nullptr)
            {
                return false;
            }

            return abstract_handler<trequest>::_next_handler->handle(request);
        }

        _target_action->execute(request);
        return true;
    }
};

template<typename trequest> class request_handler_with_command_chain final
{
private:
    abstract_handler<trequest> *_first_handler;
    abstract_handler<trequest> *_last_handler;
public:
    request_handler_with_command_chain()
        : _first_handler(nullptr), _last_handler(nullptr)
    {

    }
    ~request_handler_with_command_chain()
    {
        while (_first_handler != nullptr)
        {
            _last_handler = _first_handler;
            _first_handler = _first_handler->_next_handler;
            delete _last_handler;
        }
    }
    request_handler_with_command_chain(request_handler_with_command_chain const &) = delete;
    request_handler_with_command_chain(request_handler_with_command_chain &&) = delete;
    request_handler_with_command_chain &operator=(request_handler_with_command_chain const &) = delete;
    request_handler_with_command_chain &operator=(request_handler_with_command_chain &&) = delete;
public:
    bool handle(trequest const &request) const noexcept
    {
        if (_first_handler == nullptr)
        {
            return false;
        }

        return _first_handler->handle(request);
    }
public:
    request_handler_with_command_chain &add_handler(command<trequest> *cmd)
    {
        _last_handler = _first_handler == nullptr
            ? _first_handler = new request_handler_with_command<trequest>(cmd)
            : _last_handler->_next_handler = new request_handler_with_command<trequest>(cmd);

        return *this;
    }
};

struct delivery_info
{

};

class database_singleton
{
private:
    class stdpair_int_int_comparer final
    {
    private:
        int_comparer _int_comparer;
    public:
        int operator()(
            std::pair<int, int> const &left,
            std::pair<int, int> const &right)
        {
            auto user_id_comparison_result = _int_comparer(left.first, right.first);
            if (user_id_comparison_result != 0)
            {
                return user_id_comparison_result;
            }

            return _int_comparer(left.second, right.second);
        }
    };
private:
    static database_singleton *_instance;
public:
    static database_singleton *get_instance()
    {
        if (_instance == nullptr)
        {
            _instance = new database_singleton();
        }

        return _instance;
    }
private:
    associative_container<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_info *> *> *> *, memory *> > *_database_entrypoint;
private:
    database_singleton()
    {
        _database_entrypoint = new bplus_tree<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_info *> *> *> *, memory *>, stdstring_comparer>(10);
    }
public:
    ~database_singleton()
    {
        delete _database_entrypoint;
    }
    database_singleton(database_singleton const &) = delete;
    database_singleton(database_singleton &&) = delete;
    database_singleton &operator=(database_singleton const &) = delete;
    database_singleton &operator=(database_singleton &&) = delete;
public:
    void add_pool(
        std::string const &pool_name,
        allocator_type pool_allocator_type,
        unsigned int pool_allocator_size,
        memory_with_fit_allocation::allocation_mode pool_allocator_allocation_mode)
    {
        try
        {
            memory *allocator = nullptr;
            switch (pool_allocator_type)
            {
                case allocator_type::global_heap:
                    allocator = new memory_from_global_heap(logger_singleton::get_instance()->get_logger());
                    break;
                case allocator_type::sorted_list:
                    allocator = new memory_with_sorted_list_deallocation(pool_allocator_size, nullptr, logger_singleton::get_instance()->get_logger(), pool_allocator_allocation_mode);
                    break;
                case allocator_type::boundary_tags:
                    allocator = new memory_with_boundary_tags_deallocation(pool_allocator_size, nullptr, logger_singleton::get_instance()->get_logger(), pool_allocator_allocation_mode);
                    break;
                case allocator_type::buddy_system:
                    allocator = new memory_buddy_system(pool_allocator_size, nullptr, logger_singleton::get_instance()->get_logger(), pool_allocator_allocation_mode);
                    break;
            }
            _database_entrypoint->insert(pool_name, std::move(std::make_pair(new bplus_tree<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_info *> *> *, stdstring_comparer>(10), allocator)));
        }
        catch (search_tree<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_info *> *> *> *, memory *>, stdstring_comparer>::insertion_exception const &ex)
        {
            // TODO: ?!
        }
    }
};

//request.starts_with("");// TODO
// STARTS THERERRERERERER
class command_add_pool final : public command<std::string>
{
private:
    std::string _pool_name;
    allocator_type _pool_allocator_type;
    unsigned int _pool_allocator_size;
    memory_with_fit_allocation::allocation_mode _pool_allocator_allocation_mode;

public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_add_pool::can_execute(std::string const &request) called");

        // TODO

        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_add_pool::execute(std::string const &request) called");
        database_singleton::get_instance()->add_pool(_pool_name, _pool_allocator_type, _pool_allocator_size, _pool_allocator_allocation_mode);
    }
};

class command_remove_pool final : public command<std::string>
{
private:
    std::string _pool_name;
public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_remove_pool::can_execute(std::string const &request) called");
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_remove_pool::execute(std::string const &request) called");
    }
};

class command_add_scheme final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_add_scheme::can_execute(std::string const &request) called");
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_add_scheme::execute(std::string const &request) called");
    }
};

class command_remove_scheme final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_remove_scheme::can_execute(std::string const &request) called");
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_remove_scheme::execute(std::string const &request) called");
    }
};

class command_add_collection final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_add_collection::can_execute(std::string const &request) called");
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_add_collection::execute(std::string const &request) called");
    }
};

class command_remove_collection final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_remove_collection::can_execute(std::string const &request) called");
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_remove_collection::execute(std::string const &request) called");
    }
};

class command_add_data final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;

public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_add_data::can_execute(std::string const &request) called");
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_add_data::execute(std::string const &request) called");
    }
};

class command_get_data final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_get_data::can_execute(std::string const &request) called");
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_get_data::execute(std::string const &request) called");
    }
};

class command_get_data_between final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_get_data_between::can_execute(std::string const &request) called");
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_get_data_between::execute(std::string const &request) called");
    }
};

class command_update_data final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_update_data::can_execute(std::string const &request) called");
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_update_data::execute(std::string const &request) called");
    }
};

class command_remove_data final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_remove_data::can_execute(std::string const &request) called");
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_remove_data::execute(std::string const &request) called");
    }
};

void requests_chain_demo()
{
    auto chain = request_handler_with_command_chain<std::string>();
    chain
        .add_handler(new command_add_pool)
        .add_handler(new command_remove_pool)
        .add_handler(new command_add_scheme)
        .add_handler(new command_remove_scheme)
        .add_handler(new command_add_collection)
        .add_handler(new command_remove_collection)
        .add_handler(new command_add_data)
        .add_handler(new command_get_data)
        .add_handler(new command_get_data_between)
        .add_handler(new command_update_data)
        .add_handler(new command_remove_data);

    auto response_handled = chain.handle("pohuj");
    std::cout << "request " << (response_handled ? "" : "not ") << "handled";
}

int main()
{
    //bplus_tree_test();
    //gorschenko_bst_demo();
    //mingazova_allocator_demo();

    requests_chain_demo();

    return 0;
}