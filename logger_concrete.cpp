#include "logger_concrete.h"
#include <iostream>
#include <fstream>
#include <ctime>

std::map<std::string, std::pair<std::ofstream *, size_t> > logger_concrete::_streams =
    std::map<std::string, std::pair<std::ofstream *, size_t> >();

logger_concrete::logger_concrete(
    std::map<std::string, logger::severity> const & targets)
{
    for (auto & target : targets)
    {
        auto global_stream = _streams.find(target.first);
        std::ofstream *stream = nullptr;

        if (global_stream == _streams.end())
        {
            if (target.first != "console") // TODO: ?!
            {
                stream = new std::ofstream;
                stream->open(target.first);
            }

            _streams.insert(std::make_pair(target.first, std::make_pair(stream, 1)));
        }
        else
        {
            stream = global_stream->second.first;
            global_stream->second.second++;
        }

        _logger_streams.insert(std::make_pair(target.first, std::make_pair(stream, target.second)));
    }
}

logger_concrete::~logger_concrete()
{
    for (auto & logger_stream : _logger_streams)
    {
        auto global_stream = _streams.find(logger_stream.first);
        // TODO: check if global_stream == _streams.end()
        if (--(global_stream->second.second) == 0)
        {
            if (global_stream->second.first != nullptr)
            {
                global_stream->second.first->flush();
                global_stream->second.first->close();
                delete global_stream->second.first;
            }

            _streams.erase(global_stream);
        }
    }
}

logger const *logger_concrete::log(
    const std::string &to_log,
    logger::severity severity) const
{
    std::string cur_severity;
    if (severity == logger::severity::critical) cur_severity = "critical";
    else if (severity == logger::severity::warning) cur_severity = "warning";
    else if (severity == logger::severity::information) cur_severity = "information";
    else if (severity == logger::severity::debug) cur_severity = "debug";
    else if (severity == logger::severity::trace) cur_severity = "trace";
    else if (severity == logger::severity::error) cur_severity = "error";
    time_t now = time(0);
    tm* time_struct = localtime(&now);
    for (auto & logger_stream : _logger_streams)
    {
        if (logger_stream.second.second > severity)
        {
            continue;
        }
        std::string cur_time_string = "[date " +  std::to_string(time_struct->tm_mday) +" "+ std::to_string(time_struct->tm_hour) +":"+
                                      std::to_string(time_struct->tm_min) +":"+std::to_string(time_struct->tm_sec) + "]";
        if (logger_stream.second.first == nullptr)
        {
            std::cout << "[" << cur_severity << "]"  <<  cur_time_string << to_log << std::endl;
        }
        else
        {
            (*logger_stream.second.first) << "[" << cur_severity << "]" <<  cur_time_string << to_log << std::endl;
        }
    }

    return this;
}
