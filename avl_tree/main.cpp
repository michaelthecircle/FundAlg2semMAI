#include "../Task5/memory_buddy_system.h"
#include "../task4/boundary_tags_allocator.h"
#include "../task3/list_memory.h"
#include "../Task11/binary_search_tree.h"
#include "avl_tree.h"
#include <iostream>

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

int main()
{
    logger_builder *builder = new logger_builder_concrete();
    logger *log = builder
                      ->add_stream("file1.txt", logger::severity::trace)
                      ->add_stream("file2.txt", logger::severity::debug)
                      ->construct();

    memory *allocator = new border_descriptors_memory(log, nullptr, 10000, memory::allocate_mode::first_fit);

    auto *avl = new avl_tree<int, std::string, key_comparer>(allocator, log);

    avl->insert(1, std::move(std::string("a")));
    avl->insert(2, std::move(std::string("b")));
    avl->insert(15, std::move(std::string("o")));
    avl->insert(3, std::move(std::string("b")));
    avl->insert(4, std::move(std::string("a")));
    // auto ins = associative_container<int, std::string>::key_and_value_pair{6, std::move("c")};
    // *bst += ins;

    // avl->remove(15);
    // avl->remove(2);
    auto tree = *reinterpret_cast<binary_search_tree<int, std::string, key_comparer> *>(avl);

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

    delete avl;

    return 0;
}