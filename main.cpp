#include "logger.h"
#include "logger_builder_concrete.h"
#include "list_memory.h"
#include <list>

int main()
{
    logger_builder* build = new logger_builder_concrete();
    logger* log_m = build
            ->add_stream("sorted_list_allocator1.txt", logger::severity::trace)
            ->construct();
    memory* all1 = new list_memory(1000, memory::allocation_mode::first_fit, log_m);
    std::list<void *> allocated_blocks;
    //srand((unsigned)time(nullptr));
    srand(0);
    // std::cout<< sizeof(size_t) << " - size of size_t " << sizeof(void*) << " - size of void* " << std::endl; 8 8
    for (int i = 0; i < 10; i++)
    {
        switch (rand() % 2)
        {
            case 0:
                try
                {
                    //allocated_blocks.push_back(all1->allocate(rand() % 201 + 150));
                    allocated_blocks.push_back(all1->allocate(rand() % 101 + 50));
                }
                catch (std::bad_alloc const &ex)
                {
                    std::cout << i << " allocate failed" << std::endl;
                }
                break;
            case 1:
                try
                {
                    if (allocated_blocks.empty())
                    {
                        std::cout << i << " nothing to deallocate" << std::endl;
                        continue;
                    }

                    auto ptr_to_deallocate_index = rand() % allocated_blocks.size();
                    auto it = allocated_blocks.begin();
                    std::advance(it, ptr_to_deallocate_index);
                    all1->deallocate(*it);
                    allocated_blocks.erase(it);
                }
                catch (memory::dealloc_fail const &ex)
                {
                    std::cout << i << " deallocate failed" << std::endl;
                }
                break;
        }
    }
    while (!allocated_blocks.empty())
    {
        auto ptr_to_deallocate_index = rand() % allocated_blocks.size();
        auto it = allocated_blocks.begin();
        std::advance(it, ptr_to_deallocate_index);
        all1->deallocate(*it);
        allocated_blocks.erase(it);
    }
    delete all1;
    delete log_m;
    delete build;
    return 0;
}












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