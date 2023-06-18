#include "memory.h"
#include "not_implemented.h"


memory::memory_exception::memory_exception(const std::string &msg)
        : _msg(msg) {}

const char *memory::memory_exception::what() const noexcept
{
    return _msg.c_str();
}

void *memory::get_pointer_block(void *target_ptr) const
{
    throw not_implemented("get_pointer_block");
}
size_t memory::get_size_block(void *target_ptr) const
{
    throw not_implemented("get_size_block");
}

void *memory::get_first_avail_block() const
{
    throw not_implemented("get_first_avail_block");
}

void **memory::get_first_avail_block_ptr() const
{
    throw not_implemented("get_first_avail_block_ptr");
}

size_t memory::get_service_part_allocator_size() const
{
    throw not_implemented("get_service_part_allocator_size");
}

size_t memory::get_service_part_avail_block_size() const
{
    throw not_implemented("get_service_part_avail_block_size");
}

logger *memory::get_logger() const
{
    throw not_implemented("get_logger");
}

void memory::dump_allocator_state(bool is_allocate) const
{
    throw not_implemented("dump_allocator_state");
}

void *memory::get_right_border_of_memory_ptr() const
{
    throw not_implemented("get_right_border_of_memory_ptr");
}

std::string memory::get_pointer_address(void *target_ptr) const
{
    throw not_implemented("get_pointer_address");
}

void memory::debug_alloc(const void *target_ptr) const
{
    throw not_implemented("debug_alloc");
}

std::string memory::get_allocate_mode_string(allocate_mode method) const
{
    throw not_implemented("get_allocate_mode_string");
}

void **memory::get_first_occup_block() const
{
    throw not_implemented("get_first_occup_block");
}

size_t memory::get_allocator_size() const
{
    throw not_implemented("get_allocator_size");
}

size_t memory::get_occup_block_serv_size() const
{
    throw not_implemented("get_occup_block_serv_size");
}

void **memory::get_pointer_prev_block(void *target_ptr) const
{
    throw not_implemented("get_pointer_prev_block");
}

void **memory::get_pointer_next_block(void *target_ptr) const
{
    throw not_implemented("get_pointer_next_block");
}

std::pair<void *, size_t> memory::get_aviable_block_address_and_size(void *const previous_occupied_block_address,
                                                                     void *const next_occupied_block_address) const
{
    throw not_implemented("get_aviable_block_address_and_size");
}

size_t memory::get_avail_block_serv_size() const
{
    throw not_implemented("get_avail_block_serv_size");
}

size_t memory::get_block_avail_size(void *target_ptr) const
{
    throw not_implemented("get_block_avail_size");
}

void *memory::get_pointer_avail_prev_block(void *target_ptr) const
{
    throw not_implemented("get_pointer_avail_prev_block");
}

void *memory::get_pointer_avail_next_block(void *target_ptr) const
{
    throw not_implemented("get_pointer_avail_next_block");
}

unsigned char memory::compact_block_size_and_block_occup(size_t size_as_power_of_2, bool block_occup) const
{
    throw not_implemented("compact_block_size_and_block_occup");
}

void *memory::get_buddies(void *target_ptr) const
{
    throw not_implemented("get_buddies");
}

bool memory::is_free_block(void *target_ptr) const
{
    throw not_implemented("is_free_block");
}

memory *memory::get_memory() const
{
    throw not_implemented("get_memory");
}
