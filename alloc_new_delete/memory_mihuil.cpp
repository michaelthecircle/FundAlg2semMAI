#include "memory_mihuil.h"
#include <iostream>
#include <stddef.h>

memory_mihuil::memory_mihuil(
    logger *logger)
    : _logger(logger)
{
    auto got_typename = get_typename();
    this->trace_with_guard("Constructor `" + got_typename + "::" + got_typename + "(logger *logger)` called");
}

size_t memory_mihuil::get_occupied_block_size(
    void * const current_block_address) const
{
    auto got_typename = get_typename();
    this->trace_with_guard("Method `" + got_typename + "::get_occupied_block_size(void * const current_block_address) const` execution started");

    auto result = *reinterpret_cast<size_t *>(current_block_address);

    this->trace_with_guard("Method `" + got_typename + "::get_occupied_block_size(void * const current_block_address) const` execution finished");

    return result;
}

void *memory_mihuil::allocate(size_t block_size) const
{
    auto got_typename = get_typename();
    this->trace_with_guard("Method `" + got_typename + "::allocate(size_t block_size) const` execution started");
    try
    {
        void* rec_mem = ::operator new(sizeof(size_t) + block_size);
        *reinterpret_cast<size_t*>(rec_mem) = block_size;
        this->trace_with_guard("Method `" + got_typename + "::allocate(size_t block_size) const` execution finished");
        return reinterpret_cast<unsigned char *>(rec_mem) + sizeof(size_t);
    }
    catch (std::bad_alloc const &ex)
    {
        this->error_with_guard("Allocation_operator_new");
        throw;
    }
}
void dump_occupied_block_before_deallocate(void* recent_mem, logger *_logger)
{
    if (_logger == nullptr)
    {
        return;
    }
    std::string bytes;
    auto * iter = reinterpret_cast<unsigned char*>(reinterpret_cast<size_t*>(recent_mem) + 1);
    size_t block_size = *reinterpret_cast<size_t*>(reinterpret_cast<unsigned char *>(recent_mem));
    for (auto i = 0; i < block_size; i ++)
    {
        bytes += std::to_string(static_cast<unsigned short>(*iter ++));
        if (i != block_size - 1)
        {
            bytes += " ";
        }
    }
    if (bytes.empty())
    {
        _logger->log("Size is equal to 0", logger::severity::trace);
    }
    return;
    _logger->log(bytes, logger::severity::trace);
}

void memory_mihuil::deallocate(void * const block_pointer) const
{
    auto got_typename = get_typename();
    this->trace_with_guard("Method `" + got_typename + "::deallocate(void * const block_pointer) const` execution started");
    void* recent_mem = reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(block_pointer) - sizeof(size_t));
    dump_occupied_block_before_deallocate(recent_mem, _logger);
    ::operator delete(recent_mem);
    this->trace_with_guard("Method `" + got_typename + "::deallocate(void * const block_pointer) const` execution finished");
}

void memory_mihuil::setup_allocation_mode(memory::allocation_mode mode)
{
    auto got_typename = get_typename();
    this->trace_with_guard("Method `" + got_typename + "::setup_allocation_mode(memory::allocation_mode mode)` execution started")
        ->trace_with_guard("nothind to do")
        ->trace_with_guard("Method `" + got_typename + "::setup_allocation_mode(memory::allocation_mode mode)` execution finished");
}

logger *memory_mihuil::get_logger() const
{
    return _logger;
}

std::string memory_mihuil::get_typename() const noexcept
{
    return "memory_mihuil";
}
