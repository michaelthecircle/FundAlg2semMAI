cmake_minimum_required(VERSION 3.23)
project(zad_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
find_package(nlohmann_json 3.11.2 REQUIRED)
add_executable(zad_1
        memory.h
        memory.cpp
        not_implemented.h
        list_memory.h
        list_memory.cpp
        logger_holder.h
        logger_holder.cpp
        typename_holder.h
        logger.cpp
        logger.h
        logger_builder.cpp
        logger_builder.h
        logger_builder_concrete.cpp
        logger_builder_concrete.h
        logger_concrete.cpp
        logger_concrete.h
        main.cpp)
target_link_libraries(zad_1 PRIVATE nlohmann_json::nlohmann_json)