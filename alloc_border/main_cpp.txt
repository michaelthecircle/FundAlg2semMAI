#include <iostream>
#include "memory.h"
#include "boundary_tags_allocator.h"
#include <list>
#include <ctime>

void allocator_demo_1(
    unsigned int iterations_count)
{
    logger_builder *builder = new logger_builder_concrete();
    logger *log = builder
                      ->add_stream("file1.txt", logger::severity::trace)
                      ->add_stream("file2.txt", logger::severity::debug)
                      ->construct();

    memory *allocator = new border_descriptors_memory(log, nullptr, 1000000, memory::allocate_mode::best_fit);

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

void allocator_demo_2()
{
    logger_builder *builder = new logger_builder_concrete();
    logger *log = builder
                      ->add_stream("file1.txt", logger::severity::trace)
                      ->add_stream("file2.txt", logger::severity::debug)
                      ->construct();

    memory *allocator = new border_descriptors_memory(log, nullptr, 1000000, memory::allocate_mode::first_fit);

    int *ptr = reinterpret_cast<int *>(allocator->allocate(sizeof(int) * 25));
    auto *ptr2 = allocator->allocate(100);
    for (int i = 0; i < 25; i++)
    {
        ptr[i] = i;
    }
    allocator->deallocate(ptr);
    ptr = reinterpret_cast<int *>(allocator->allocate(99));
    allocator->deallocate(ptr);
    allocator->deallocate(ptr2);
    delete allocator;
    delete log;
    delete builder;
}

int main()
{
    allocator_demo_1(1000000);
}