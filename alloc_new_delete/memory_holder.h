#ifndef MAIN_CPP_MEMORY_HOLDER_H
#define MAIN_CPP_MEMORY_HOLDER_H
#include "memory.h"

class memory_holder
{

public:

    virtual ~memory_holder() noexcept = default;

public:

    void *allocate_with_guard(
            size_t size) const
    {
        auto *allocator = get_memory();

        return allocator == nullptr
               ? ::operator new(size)
               : allocator->allocate(size);
    }

    void deallocate_with_guard(
            void *block_pointer) const
    {
        auto *allocator = get_memory();

        if (allocator == nullptr)
        {
            ::operator delete(block_pointer);
        }
        else
        {
            try
            {
                allocator->deallocate(block_pointer);
            }
            catch (std::exception())
            {
                ::operator delete(block_pointer);
            }
        }
    }

protected:

    virtual memory *get_memory() const noexcept = 0;

};
#endif //MAIN_CPP_MEMORY_HOLDER_H

