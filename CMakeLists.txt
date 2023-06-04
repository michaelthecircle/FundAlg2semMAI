cmake_minimum_required(VERSION 3.23)
project(zad_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
find_package(nlohmann_json 3.11.2 REQUIRED)
add_executable(zad_1
        memory.h
        memory.cpp
        not_implemented.h
        alloc_list/list_memory.h
        alloc_list/list_memory.cpp
        logger/logger_holder.h
        logger/logger_holder.cpp
        typename_holder.h
        logger/logger.cpp
        logger/logger.h
        logger/logger_builder.cpp
        logger/logger_builder.h
        logger/logger_builder_concrete.cpp
        logger/logger_builder_concrete.h
        logger/logger_concrete.cpp
        logger/logger_concrete.h
        alloc_new_delete/memory_holder.h
        alloc_new_delete/memory_mihuil.cpp
        alloc_new_delete/memory_mihuil.h
        binary_search_tree/binary_search_tree.h
        kursach_puk_puk/collection.h
        kursach_puk_puk/database.h
        kursach_puk_puk/definition_of_delivery.h
        kursach_puk_puk/scheme.h
        comparers/stdstring_comparer.h
        kursach_puk_puk/trees_names.h
        kursach_puk_puk/file_parcer.h
        main.cpp)
target_link_libraries(zad_1 PRIVATE nlohmann_json::nlohmann_json)