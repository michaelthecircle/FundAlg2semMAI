#ifndef _MEMORY
#define _MEMORY
#include <stddef.h>
#include <iostream>

class memory{
public:
    enum class allocation_mode{
        best_fit,
        first_fit,
        worst_fit
    };
    class dealloc_fail final : public std::exception
    {
    public:
        char const* what()const noexcept override
        {
            return "Failed to deallocate";
        }
    };
public:
    virtual size_t get_occupied_block_size(void * const current_block_address) const;
    virtual void * allocate(size_t block_size) const = 0;
    virtual void deallocate(void * const block_pointer) const = 0;
    virtual void setup_allocation_mode(memory::allocation_mode mode);
    virtual void* const get_first_aviable_block()const;
    virtual void** get_first_aviable_block_ptr()const;
    virtual void* get_right_border_of_memory_ptr() const; //add
    virtual size_t get_current_aviable_block_size(void const*) const;
    virtual void* get_next_aviable_block(void*) const;
    virtual size_t get_service_part_aviable_block_size()const;
    virtual size_t get_service_part_occupied_block_size()const;
    virtual size_t get_service_part_allocator_size() const{return 0;}
    virtual void log_of_changed_size() const{};
public:
    virtual ~memory() noexcept;
};
//for memory_mihuil changed get_service and log_of_changed_size to virtual from pure virtual
#endif //_MEMORY
