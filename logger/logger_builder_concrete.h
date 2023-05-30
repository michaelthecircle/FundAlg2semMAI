#ifndef SANDBOX_CPP_LOGGER_BUILDER_CONCRETE_H
#define SANDBOX_CPP_LOGGER_BUILDER_CONCRETE_H

#include <map>
#include "logger_builder.h"

class logger_builder_concrete final : public logger_builder
{

private:

    std::map<std::string, logger::severity> _construction_info;

public:

    logger_builder *add_stream(std::string const &, logger::severity) override;
    logger_builder *json_stream(std::string const &) override;
    logger *construct() const override;

};


#endif //SANDBOX_CPP_LOGGER_BUILDER_CONCRETE_H
