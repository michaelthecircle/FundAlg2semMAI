#include "logger_builder_concrete.h"
#include "logger_concrete.h"
#include <nlohmann/json.hpp>
#include <fstream>

logger_builder *logger_builder_concrete::add_stream(
    std::string const &path,
    logger::severity severity)
{
    _construction_info[path] = severity;

    return this;
}

logger_builder *logger_builder_concrete::json_stream(std::string const &json_file_name)
{
    logger::severity cur_severity;
    std::ifstream f(json_file_name);
    nlohmann::json data = nlohmann::json::parse(f);
    for (auto& element : data.items())
    {
        if (element.value() == "trace") cur_severity = logger::severity::trace;
        else if (element.value() == "debug") cur_severity = logger::severity::debug;
        else if (element.value() == "information") cur_severity = logger::severity::information;
        else if (element.value() == "warning") cur_severity = logger::severity::warning;
        else if (element.value() == "trace") cur_severity = logger::severity::trace;
        else if (element.value() == "critical") cur_severity = logger::severity::critical;
        _construction_info[element.key()] = cur_severity;
    }
    return this;
}

logger *logger_builder_concrete::construct() const
{
    return new logger_concrete(_construction_info);
}