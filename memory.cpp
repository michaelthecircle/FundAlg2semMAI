#include "memory.h"
#include "not_implemented.h"

void memory::setup_allocation_mode(memory::allocation_mode mode)
{
    throw not_implemented();
}
size_t memory::get_current_aviable_block_size(void const*) const
{
    throw not_implemented();
}
size_t memory::get_service_part_aviable_block_size()const
{
    throw not_implemented();
}
size_t memory::get_service_part_occupied_block_size()const
{
    throw not_implemented();
}

memory::~memory() noexcept
{

}

void* const memory::get_first_aviable_block()const
{
    throw not_implemented();
}
void** memory::get_first_aviable_block_ptr()const
{
    throw not_implemented();
}
void* memory::get_next_aviable_block(void*) const
{
    throw not_implemented();
}
size_t memory::get_occupied_block_size(void * const current_block_address) const
{
    throw not_implemented();
}

void* memory::get_right_border_of_memory_ptr() const
{
    throw not_implemented();
}

