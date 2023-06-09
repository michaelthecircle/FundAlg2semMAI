#include "./logger/logger_builder.h"
#include "./logger/logger_builder_concrete.h"
//#include "./comparers/int_comparer.h"
#include "./b_plus_tree/bplus_tree.h"
#include "./allocator_names/allocator_type.h"
#include "./alloc_list/list_memory.h"
#include "./alloc_new_delete/memory_mihuil.h"
#include <iostream>
#include <random>
#include "./not_implemented.h"
#include "comparers/int_comparer.h"

void bplus_tree_test()
{
    srand(12345);
    logger_builder *builder = new logger_builder_concrete();
    auto *allocator_logger = builder
            ->add_stream("global_heap_allocator.txt", logger::severity::debug)
            ->construct();
    delete builder;
    memory *global_heap_allocator = new memory_mihuil(allocator_logger);

    bplus_tree<int, std::string, int_comparer> b_plus_tree_for_test(3, global_heap_allocator);

    auto i = 0;
    auto const values_count = 10;
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
            b_plus_tree_for_test.insert(v[i], std::move(std::to_string(v[i] * 2 + 1)));
            std::cout << "State after iteration #" << i << " == " << std::endl;
            b_plus_tree_for_test.iterate_terminal_nodes_list();
            std::cout << std::endl;
        }
        catch (search_tree<int, std::string, int_comparer>::insertion_exception const &ex)
        {
            std::cout << "cannot insert" << std::endl;
        }
        catch (not_implemented const &ex)
        {
            std::cout << "allocator died" << std::endl;
        }
    }

    std::cout << "State after iteration #" << i << ':' << std::endl;
    b_plus_tree_for_test.iterate_tree();

    std::cout << std::endl << std::endl << "Terminal nodes list after iteration #" << i << ':' << std::endl;
    b_plus_tree_for_test.iterate_terminal_nodes_list();

    delete global_heap_allocator;
    delete allocator_logger;
}

int main()
{
    bplus_tree_test();
    return 0;
}










