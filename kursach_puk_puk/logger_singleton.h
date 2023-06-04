#ifndef MAIN_CPP_LOGGER_SINGLETON_H
#define MAIN_CPP_LOGGER_SINGLETON_H

#include "./logger/logger_holder.h"
#include "./logger/logger_builder_concrete.h"

class logger_singleton : public logger_holder
{
private:
    static logger_singleton *_instance;
public:
    static logger_singleton *get_instance()
    {
        if (_instance == nullptr)
        {
            _instance = new logger_singleton("logger_settings.json");
        }

        return _instance;
    }
private:
    logger *_logger;
private:
    logger_singleton(std::string const &logger_config_file_path)
    {
        // TODO: use json

        auto *builder = new logger_builder_concrete;
        builder
                ->add_stream("logs.txt", logger::severity::trace);
        _logger = builder->construct();
        delete builder;
    }
public:
    ~logger_singleton() { delete _logger; }
    logger_singleton(logger_singleton const &) = delete;
    logger_singleton(logger_singleton &&) = delete;
    logger_singleton &operator=(logger_singleton const &) = delete;
    logger_singleton &operator=(logger_singleton &&) = delete;
public:
    logger *get_logger() const noexcept override
    {
        return _logger;
    }
};


#endif //MAIN_CPP_LOGGER_SINGLETON_H
