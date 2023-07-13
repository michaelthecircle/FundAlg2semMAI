#include "logger/logger.h"
#include "logger/logger_builder_concrete.h"
//#include "alloc_list/list_memory.h"
#include <list>
#include <iostream>
#include "./Parse/file_reader.h"
#include "./b_plus_tree/bplus_tree.h"
#include <random>
#include "./comparers/int_comparer.h"
//#include "./alloc_new_delete/memory_mihuil.h"
#include "./alloc_border/boundary_tags_allocator.h"
#include "./alloc_buddies/budy_system.h"
#include "./binary_search_tree/binary_search_tree_2.h"
#include "./avl_tree/avl_tree.h"
#include "./splay_tree/splay_tree.h"
#include "kursach_puk_puk/logger_singleton.h"
#include "Parse/file_reader.h"

//bplus_tree_test
/*void bplus_tree_test()
{
    srand(12345);
    logger_builder *builder = new logger_builder_concrete();
    auto *sorted_list_allocator_logger = builder
            ->add_stream("sorted list allocator.txt", logger::severity::debug)->construct();
    delete builder;
    memory *sorted_list_allocator = new memory_mihuil(sorted_list_allocator_logger);

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
        //catch (memory::memory_exception const &ex)
        //{
        //    std::cout << "allocator puk puk((9(9" << std::endl;
        //}
    }

    std::cout << "State after iteration #" << i << ':' << std::endl;
    denis.iterate_tree();

    std::cout << std::endl << std::endl << "Terminal nodes list after iteration #" << i << ':' << std::endl;
    //denis.iterate_terminal_nodes_list();

    std::cout << '\"' << denis.get(123) << '\"';

    delete sorted_list_allocator;
    delete sorted_list_allocator_logger;
}*/
/*
void allocator_demo_1(
        unsigned int iterations_count)
{
    logger_builder *builder = new logger_builder_concrete();
    logger *log = builder
            ->add_stream("border_descriptors_trace.txt", logger::severity::trace)
            ->add_stream("border_descriptors_debug.txt", logger::severity::debug)
            ->construct();
    memory *allocator = new border_descriptors_memory(log, nullptr, 10000000, memory::allocate_mode::best_fit);
    std::list<void *> allocated_blocks;
    srand((unsigned)time(nullptr));
    for (auto i = 0; i < iterations_count; i++)
    {
        switch (rand() % 2)
        {
            case 0:
            case 1:
                try
                {
                    allocated_blocks.push_front(allocator->allocate(rand() % 251 + 50));
                    std::cout << "allocation succeeded" << std::endl;
                }
                catch (std::bad_alloc const &ex)
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

    delete allocator;
    delete log;
    delete builder;
}

void allocator_demo_2(
        unsigned int iterations_count)
{
    logger_builder *builder = new logger_builder_concrete();
    logger *log = builder
            ->add_stream("buddies_system_trace.txt", logger::severity::trace)
            ->add_stream("buddies_system_debug.txt", logger::severity::debug)
            ->construct();
    memory *allocator = new memory_buddy_system(log, nullptr, 30, memory::allocate_mode::first_fit);
    std::list<void *> allocated_blocks;
    srand((unsigned)time(nullptr));
    for (auto i = 0; i < iterations_count; i++)
    {
        switch (rand() % 2)
        {
            case 0:
            case 1:
                try
                {
                    allocated_blocks.push_front(allocator->allocate(rand() % 251 + 50));
                    std::cout << "allocation succeeded" << std::endl;
                }
                catch (std::bad_alloc const &ex)
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

    delete allocator;
    delete log;
    delete builder;
}

void tree_test(std::string tree_type)
{
    * tree types:
    "bst" - bst
    "avl" - avl
    "splay" - splay

    class functor_int
    {
    public:
        int operator()(int first, int second)
        {
            return first - second;
        }
    };

    class key_comparer
    {
    public:
        int operator()(int first, int second)
        {
            return first - second;
        }

        int operator()(std::string first, std::string second)
        {
            if (first > second)
                return 1;
            else if (first < second)
                return -1;
            else
                return 0;
        }
    };
    logger_builder *builder = new logger_builder_concrete();
    logger *log = builder
            ->add_stream("bst_test_trace.txt", logger::severity::trace)
            ->add_stream("bst_test_debug.txt", logger::severity::debug)
            ->construct();

    memory *allocator = new border_descriptors_memory(log, nullptr, 10000, memory::allocate_mode::first_fit);
    if (tree_type == "bst")
    {
        associative_container<int, std::string> *bst = new binary_search_tree<int, std::string, key_comparer>(allocator, log);

        bst->insert(1, std::move(std::string("a")));
        bst->insert(2, std::move(std::string("b")));
        bst->insert(15, std::move(std::string("o")));
        bst->insert(3, std::move(std::string("b")));
        bst->insert(4, std::move(std::string("a")));

        auto tree = *reinterpret_cast<binary_search_tree<int, std::string, key_comparer> *>(bst);

        auto end_infix = tree.end_infix();

        for (auto it = tree.begin_infix(); it != end_infix; ++it)
        {
            for (auto i = 0; i < std::get<0>(*it); i++)
            {
                std::cout << "  ";
            }
            std::cout << "Key = \"" << std::get<1>(*it) << "\", Value = \"" << std::get<2>(*it) << "\"" << std::endl;
        }
        std::cout << std::endl;

        delete bst;
    }
    else if (tree_type == "avl")
    {
        auto *avl = new avl_tree<int, std::string, key_comparer>(allocator, log);

        avl->insert(1, std::move(std::string("a")));
        avl->insert(2, std::move(std::string("b")));
        avl->insert(15, std::move(std::string("o")));
        avl->insert(3, std::move(std::string("b")));
        avl->insert(4, std::move(std::string("a")));

        auto tree = *reinterpret_cast<binary_search_tree<int, std::string, key_comparer> *>(avl);

        auto end_infix = tree.end_infix();

        for (auto it = tree.begin_infix(); it != end_infix; ++it)
        {
            for (auto i = 0; i < std::get<0>(*it); i++)
            {
                std::cout << "  ";
            }
            std::cout << "Key = \"" << std::get<1>(*it) << "\", Value = \"" << std::get<2>(*it) << "\"" << std::endl;
        }
        std::cout << std::endl;

        delete avl;
    }
    /*else if (tree_type == "splay")
    {
        logger_builder *builder = new logger_builder_concrete();
        logger *log = builder
                ->add_stream("file1.txt", logger::severity::trace)
                ->add_stream("file2.txt", logger::severity::debug)
                ->construct();

        memory *allocator = new border_descriptors_memory(log, nullptr, 100000, memory::allocate_mode::first_fit);

        associative_container<int, std::string> *splay = new splay_tree<int, std::string, key_comparer>(allocator, log);

        splay->insert(1, std::move(std::string("a")));
        std::cout << "insert complated" << std::endl;
        splay->insert(2, std::move(std::string("b")));
        std::cout << "insert complated" << std::endl;
        splay->insert(15, std::move(std::string("o")));
        std::cout << "insert complated" << std::endl;
        splay->insert(3, std::move(std::string("b")));
        std::cout << "insert complated" << std::endl;
        splay->insert(4, std::move(std::string("a")));
        std::cout << "insert complated" << std::endl;

        // auto ins = associative_container<int, std::string>::key_and_value_pair{6, std::move("c")};
        // *bst += ins;

        splay->remove(15);
        // avl->remove(2);
        auto tree = *reinterpret_cast<binary_search_tree<int, std::string, key_comparer> *>(splay);

        auto end_infix = tree.end_infix();

        for (auto it = tree.begin_infix(); it != end_infix; ++it)
        {
            for (auto i = 0; i < std::get<0>(*it); i++)
            {
                std::cout << "  ";
            }
            std::cout << "Key = \"" << std::get<1>(*it) << "\", Value = \"" << std::get<2>(*it) << "\"" << std::endl;
            // std::cout << "lol" << std::endl;
            // for (int i = 0; i < std::get<0>(*it); i++)
            // {
            //     std::cout << "  ";
            // }
            // std::cout << std::get<2>(*it) << " " << std::endl;
        }
        std::cout << std::endl;

        delete splay;
    }

}
*/


int main(int argc, char *argv[])
{

    return 0;
}




//    TASK TESTS 4-5 1 2 11 - last
/* for 4-5 tasks:
     //allocator_demo_1(100);
    //allocator_demo_2(100);
*/

/* for 1st task
 * logger_builder* builder0 = new logger_builder_concrete();
    logger* lg1 = builder0->json_stream("aboba.json")->construct();
    lg1->log("pukpuk", logger::severity::trace);
    lg1->log("srenk", logger::severity::debug);
    logger_builder* builder = new logger_builder_concrete();
    logger *constructed_logger = builder
        ->add_stream("file1.txt", logger::severity::critical)
        ->add_stream("file2.txt", logger::severity::debug)
        ->add_stream("file3.txt", logger::severity::trace)
        ->construct();

    logger *constructed_logger_2 = builder
        ->add_stream("file4.txt", logger::severity::warning)
        ->construct();

    constructed_logger
        ->log("kek lol 123", logger::severity::information);

    constructed_logger_2
        ->log("123 kek lol", logger::severity::error);

    delete constructed_logger_2;
    delete constructed_logger;
    delete builder;
    */

/* for 2d task
    logger_builder* builder_mem = new logger_builder_concrete();
    logger *log_mem = builder_mem->add_stream("mem.txt", logger::severity::trace)->construct();
    memory_mihuil memoryMihuil(log_mem);
    int const size = 0;
    int* sobaka = reinterpret_cast<int*>(memoryMihuil.allocate(size));
    for (int i = 0; i < size/(sizeof (int)); i ++)
    {
        sobaka[i] = i;
    }
    memoryMihuil.deallocate(sobaka);
    delete builder_mem; */

/*for 11th task
    ---------------------------------------------------------in main:::::
    logger_builder *builder = new logger_builder_concrete();
    logger *log = builder
                      ->add_stream("file1.txt", logger::severity::trace)
                      ->add_stream("file2.txt", logger::severity::debug)
                      ->construct();

    memory *allocator = new border_descriptors_memory(log, nullptr, 10000, memory::allocate_mode::first_fit);

    associative_container<int, std::string> *bst = new binary_search_tree<int, std::string, key_comparer>(allocator, log);

    bst->insert(1, std::move(std::string("a")));
    bst->insert(2, std::move(std::string("b")));
    bst->insert(15, std::move(std::string("o")));
    bst->insert(3, std::move(std::string("b")));
    bst->insert(4, std::move(std::string("a")));
    // auto ins = associative_container<int, std::string>::key_and_value_pair{6, std::move("c")};
    // *bst += ins;

    // bst->remove(15);
    auto tree = *reinterpret_cast<binary_search_tree<int, std::string, key_comparer> *>(bst);

    auto end_infix = tree.end_infix();

    for (auto it = tree.begin_infix(); it != end_infix; ++it)
    {
        for (auto i = 0; i < std::get<0>(*it); i++)
        {
            std::cout << "  ";
        }
        std::cout << "Key = \"" << std::get<1>(*it) << "\", Value = \"" << std::get<2>(*it) << "\"" << std::endl;
        // std::cout << "lol" << std::endl;
        // for (int i = 0; i < std::get<0>(*it); i++)
        // {
        //     std::cout << "  ";
        // }
        // std::cout << std::get<2>(*it) << " " << std::endl;
    }
    std::cout << std::endl;

    delete bst;

    ----------------------------------------------------------also ::
    class functor_int
{
public:
    int operator()(int first, int second)
    {
        return first - second;
    }
};

class key_comparer
{
public:
    int operator()(int first, int second)
    {
        return first - second;
    }

    int operator()(std::string first, std::string second)
    {
        if (first > second)
            return 1;
        else if (first < second)
            return -1;
        else
            return 0;
    }
};
*/