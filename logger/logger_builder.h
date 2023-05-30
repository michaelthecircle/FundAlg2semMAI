#ifndef SANDBOX_CPP_LOGGER_BUILDER_H
#define SANDBOX_CPP_LOGGER_BUILDER_H

#include <iostream>
#include "logger.h"

class logger_builder
{

public:

    virtual logger_builder *add_stream(std::string const &, logger::severity) = 0;
    virtual logger_builder *json_stream(std::string const &) = 0;
    virtual logger *construct() const = 0;

public:

    virtual ~logger_builder() noexcept;

};

#endif //SANDBOX_CPP_LOGGER_BUILDER_H
