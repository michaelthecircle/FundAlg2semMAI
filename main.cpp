#include "logger/logger.h"
#include "logger/logger_builder_concrete.h"
#include "alloc_list/list_memory.h"
#include <list>
#include "./alloc_new_delete/memory_mihuil.h"
#include "binary_search_tree/binary_search_tree.h"
#include "./b_plus_tree//associative_container.h"
int main()
{
    logger_builder* build_mem = new logger_builder_concrete();
    logger* log_m = build_mem
            ->add_stream("memory.txt", logger::severity::trace)
            ->construct();
    logger_builder* build_associative = new logger_builder_concrete();
    logger* log_associative = build_associative
            ->add_stream("associative.txt", logger::severity::trace)
            ->construct();
    memory* all_mem = new memory_mihuil(log_m);
    memory* all_associative = new memory_mihuil(log_associative);

    delete all_mem;
    delete build_mem;
    delete build_associative;
    delete log_m;
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