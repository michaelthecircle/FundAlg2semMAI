#include "budy_system.h"
#include <iostream>
#include <cmath>
#include <cstring>
#include <climits>
#include <bitset>

void *memory_buddy_system::get_first_avail_block() const
{
    return *get_first_avail_block_ptr();
}

void **memory_buddy_system::get_first_avail_block_ptr() const
{
    return reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_all_memory) + get_service_part_allocator_size() - sizeof(void *));
}

void *memory_buddy_system::get_pointer_avail_prev_block(void *target_ptr) const
{
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(target_ptr) + 1);
}

void *memory_buddy_system::get_pointer_avail_next_block(void *target_ptr) const
{
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(target_ptr) + 1 + sizeof(void *));
}

size_t memory_buddy_system::get_service_part_allocator_size() const
{
    return sizeof(size_t) + sizeof(logger *) + sizeof(memory *) + sizeof(memory::allocate_mode) + sizeof(void *);
}

unsigned char memory_buddy_system::compact_block_size_and_block_occup(size_t size_as_power_of_2, bool block_occup) const
{
    return (size_as_power_of_2 << 1) | (block_occup & 1);
}

size_t memory_buddy_system::get_block_avail_size(void *target_ptr) const
{

    return 1 << (*reinterpret_cast<unsigned char *>(target_ptr) >> 1);
}

size_t memory_buddy_system::get_occup_block_serv_size() const
{
    return sizeof(unsigned char);
}

logger *memory_buddy_system::get_logger() const noexcept
{
    return *reinterpret_cast<logger **>(reinterpret_cast<unsigned char *>(_all_memory) + sizeof(size_t));
}

memory *memory_buddy_system::get_memory() const
{
    return *reinterpret_cast<memory **>(reinterpret_cast<unsigned char *>(_all_memory) + sizeof(size_t) + sizeof(logger *) + sizeof(memory::allocate_mode));
}

std::string memory_buddy_system::get_allocate_mode_string(allocate_mode method) const
{
    std::string allocate_mode;
    switch (method)
    {
        case memory::allocate_mode::first_fit:
            allocate_mode = "the first fit method";
            break;

        case memory::allocate_mode::best_fit:
            allocate_mode = "the best fit method";
            break;
        default:
            allocate_mode = "the worst fit method";
            break;
    }
    return allocate_mode;
}

std::string memory_buddy_system::get_pointer_address(void *target_ptr) const
{

    char str[20];
    memset(str, 0, 20);
    sprintf(str, "%p", target_ptr);
    std::string add;
    add.append(str);
    return add;
}

void memory_buddy_system::debug_alloc(const void *target_ptr) const
{
    logger *log = get_logger();
    if (log == nullptr)
    {
        return;
    }
    unsigned char *ptr = reinterpret_cast<unsigned char *>(reinterpret_cast<unsigned char *>(const_cast<void *>(target_ptr)) + 1);
    size_t size = get_block_avail_size(const_cast<void *>(target_ptr));
    std::string buff;
    for (int i = 0; i < size; i++)
    {
        unsigned short tmp = static_cast<unsigned short>(*ptr);
        buff.append(std::to_string(tmp) + ' ');
        ptr++;
    }
    std::string add = get_pointer_address(const_cast<void *>(target_ptr));
    std::string massege = "Block at address " + add + " state before deallocation:\n[ " + buff + "]";
    log->log(massege, logger::severity::debug);
}

void *memory_buddy_system::get_buddies(void *target_ptr) const
{
    if (get_block_avail_size(target_ptr) == *reinterpret_cast<size_t *>(_all_memory))
    {
        return nullptr;
    }

    void *first_block = reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(_all_memory) + get_service_part_allocator_size());
    size_t size_block = get_block_avail_size(target_ptr);
    size_t relative_add = reinterpret_cast<unsigned char *>(target_ptr) - reinterpret_cast<unsigned char *>(first_block);
    size_t result_xor = relative_add ^ size_block;

    return reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(first_block) + result_xor);
}

bool memory_buddy_system::is_free_block(void *target_ptr) const
{
    return *reinterpret_cast<unsigned char *>(target_ptr) & 1;
}

void memory_buddy_system::dump_allocator_state(bool is_allocate) const noexcept
{
    {
        auto *log = get_logger();
        if (log == nullptr)
        {
            return;
        }

        auto allocator_memory_size = *reinterpret_cast<size_t *>(_all_memory);
        std::string constructed_state_string("|");
        auto *current_aviable_block = get_first_avail_block();
        auto *current_any_block = reinterpret_cast<unsigned char *>(_all_memory) + get_service_part_allocator_size();

        while (current_any_block - reinterpret_cast<unsigned char *>(_all_memory) < allocator_memory_size)
        {
            size_t any_block_size = 0;
            if (current_any_block == current_aviable_block)
            {
                constructed_state_string += "avlbl " + std::to_string(any_block_size = get_block_avail_size(current_any_block)) + '|';
                current_aviable_block = get_pointer_avail_next_block(current_aviable_block);
            }
            else
            {
                constructed_state_string += "occpd " + std::to_string(any_block_size = get_block_avail_size(current_any_block)) + '|';
            }

            current_any_block += any_block_size;
        }

        log->log(std::string("Memory state after ") + (is_allocate ? "" : "de") + "allocation operation: " + constructed_state_string, logger::severity::trace);
    }
}
size_t memory_buddy_system::get_avail_block_serv_size() const
{
    return sizeof(unsigned char) + 2 * sizeof(void *);
}

void *memory_buddy_system::allocate(size_t requested_block_size) const
{
    auto alloc_method = *reinterpret_cast<memory::allocate_mode *>(reinterpret_cast<unsigned char *>(_all_memory) + sizeof(size_t) + sizeof(logger *));
    void *curr_block = get_first_avail_block();
    size_t occup_block_serv_size = get_occup_block_serv_size();
    void *target_block = nullptr;

    if (requested_block_size < ((sizeof(void *) << 2) - 1))
    {
        this->debug_with_guard("Size has changed: " + std::to_string(requested_block_size) + "->" + std::to_string((sizeof(void *) << 2) - 1));
        requested_block_size = ((sizeof(void *) << 2) - 1);
    }

    size_t curr_block_size;

    while (curr_block != nullptr)
    {

        void *next_block = get_pointer_avail_next_block(curr_block);
        curr_block_size = get_block_avail_size(curr_block);

        if (curr_block_size >= requested_block_size + occup_block_serv_size)
        {
            if (alloc_method == memory::allocate_mode::first_fit ||
                alloc_method == memory::allocate_mode::best_fit && (target_block == nullptr || get_block_avail_size(target_block) > curr_block_size) ||
                alloc_method == memory::allocate_mode::worst_fit && (target_block == nullptr || get_block_avail_size(target_block) < curr_block_size))
            {
                target_block = curr_block;
            }

            if (alloc_method == memory::allocate_mode::first_fit)
            {
                break;
            }
        }

        curr_block = next_block;
    }

    if (target_block == nullptr)
    {
        throw std::bad_alloc();
    }

    size_t target_block_size = get_block_avail_size(target_block);

    while ((target_block_size >> 1) >= requested_block_size + occup_block_serv_size)
    {
        auto *const left_buddy_address = reinterpret_cast<unsigned char *>(target_block);
        auto *const right_buddy_address = left_buddy_address + (target_block_size >>= 1);

        *right_buddy_address = *left_buddy_address = ((*left_buddy_address >> 1) - 1) << 1;

        auto *const next_block = get_pointer_avail_next_block(left_buddy_address);

        if (next_block != nullptr)
        {
            *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(next_block) + 1) = reinterpret_cast<void *>(right_buddy_address);
        }

        *(reinterpret_cast<void **>(right_buddy_address + 1) + 1) = next_block;
        *(reinterpret_cast<void **>(left_buddy_address + 1) + 1) = reinterpret_cast<void *>(right_buddy_address);
        *reinterpret_cast<void **>(right_buddy_address + 1) = reinterpret_cast<void *>(left_buddy_address);
    }

    auto const target_block_size_space = reinterpret_cast<unsigned char *>(target_block);
    *target_block_size_space = compact_block_size_and_block_occup(static_cast<size_t>(std::log2(target_block_size)), true);

    auto *const prev_to_target_block = get_pointer_avail_prev_block(target_block);
    auto *const next_to_target_block = get_pointer_avail_next_block(target_block);

    if (prev_to_target_block != nullptr)
    {
        *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(prev_to_target_block) + sizeof(unsigned char) + sizeof(void *)) = next_to_target_block;
        if (next_to_target_block != nullptr)
        {
            *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(next_to_target_block) + 1) = prev_to_target_block;
        }
    }
    else
    {
        *get_first_avail_block_ptr() = next_to_target_block;
        if (next_to_target_block != nullptr)
        {
            *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(next_to_target_block) + 1) = nullptr;
        }
    }
    this->trace_with_guard("Allocate is " + get_allocate_mode_string(alloc_method));
    dump_allocator_state(true);

    return reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(target_block) + 1);
}

void memory_buddy_system::deallocate(void *block_to_deallocate_address) const
{

    auto curr_block = reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(block_to_deallocate_address) - 1);
    debug_alloc(curr_block);
    void *buddies = get_buddies(curr_block);

    if (buddies != nullptr && (get_block_avail_size(curr_block) == get_block_avail_size(buddies)) && (is_free_block(buddies) == false))
    {
        while (buddies != nullptr && (get_block_avail_size(buddies) == get_block_avail_size(curr_block)) && (is_free_block(buddies) == false))
        {
            if (buddies < curr_block)
            {

                if (is_free_block(curr_block) == false)
                {
                    auto *const next_avail_block_for_target = get_pointer_avail_next_block(curr_block);
                    *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(buddies) + 1 + sizeof(void *)) = next_avail_block_for_target;
                    if (next_avail_block_for_target != nullptr)
                    {
                        *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(next_avail_block_for_target) + 1) = buddies;
                    }
                }
                *reinterpret_cast<unsigned char *>(buddies) = ((*reinterpret_cast<unsigned char *>(buddies) >> 1) + 1) << 1;
                curr_block = buddies;
            }
            else
            {
                void *prev_block = get_pointer_avail_prev_block(buddies);
                void *next_block = get_pointer_avail_next_block(buddies);

                if (is_free_block(curr_block) == true)
                {

                    *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(curr_block) + 1) = prev_block;
                    if (prev_block != nullptr)
                    {
                        *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(prev_block) + 1 + sizeof(void *)) = curr_block;
                    }
                    else
                    {
                        *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_all_memory) + get_service_part_allocator_size() - sizeof(void *)) = curr_block;
                    }
                }

                *reinterpret_cast<unsigned char *>(curr_block) = ((*reinterpret_cast<unsigned char *>(curr_block) >> 1) + 1) << 1;
                *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(curr_block) + 1 + sizeof(void *)) = next_block;
                if (next_block != nullptr)
                {
                    *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(next_block) + 1) = curr_block;
                }
            }

            buddies = get_buddies(curr_block);
        }
    }
    else
    {
        void *prev_block = nullptr;
        void *next_block = get_first_avail_block();

        while (next_block != nullptr && next_block < curr_block)
        {
            prev_block = next_block;
            next_block = get_pointer_avail_next_block(next_block);
        }
        // *reinterpret_cast<unsigned char *>(curr_block) = compact_block_size_and_block_occup(static_cast<size_t>(std::log2(get_block_avail_size(curr_block))), true);
        *(reinterpret_cast<unsigned char *>(curr_block)) &= ~1;
        *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(curr_block) + 1) = prev_block;
        *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(curr_block) + 1 + sizeof(void *)) = next_block;

        if (prev_block != nullptr)
        {
            *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(prev_block) + 1 + sizeof(void *)) = curr_block;
        }
        else
        {
            auto *const first_avail_block_ptr = get_first_avail_block_ptr();
            *first_avail_block_ptr = curr_block;
        }

        if (next_block != nullptr)
        {
            *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(next_block) + 1) = curr_block;
        }
    }

    dump_allocator_state(false);
}

memory_buddy_system::memory_buddy_system(logger *log, memory *allocator, size_t size_as_power_of_2, allocate_mode method)
{
    if (size_as_power_of_2 > 127)
    {
        throw std::logic_error("Overflow");
    }
    size_t target_size = 1 << size_as_power_of_2;

    if (target_size < 1 + 2 * sizeof(void *))
    {
        throw std::logic_error("Low memory");
    }

    target_size += get_service_part_allocator_size();

    _all_memory = allocator == nullptr ? ::operator new(target_size)
                                       : allocator->allocate(target_size);

    auto total_size = reinterpret_cast<size_t *>(_all_memory);
    *total_size = target_size - get_service_part_allocator_size();

    auto *logger_space = reinterpret_cast<logger **>(total_size + 1);
    *logger_space = log;

    auto *mode_space = reinterpret_cast<memory::allocate_mode *>(logger_space + 1);
    *mode_space = method;

    auto *allocator_space = reinterpret_cast<memory **>(mode_space + 1);
    *allocator_space = allocator;

    auto *first_aviable_block = reinterpret_cast<void **>(allocator_space + 1);
    *first_aviable_block = first_aviable_block + 1;

    auto *size_and_aviability = reinterpret_cast<unsigned char *>(*first_aviable_block);
    *size_and_aviability = static_cast<unsigned char>(size_as_power_of_2 << 1);

    auto *previous_to_single_block_ptr = reinterpret_cast<void **>(size_and_aviability + 1);
    auto *next_to_single_block_ptr = previous_to_single_block_ptr + 1;

    *previous_to_single_block_ptr = *next_to_single_block_ptr = nullptr;

    this->trace_with_guard("Buddy system allocator initialized!");
}

memory_buddy_system::~memory_buddy_system()
{
    auto *outer_allocator = *reinterpret_cast<memory **>(reinterpret_cast<unsigned char *>(_all_memory) + sizeof(size_t) + sizeof(logger *) + sizeof(memory::allocate_mode));
    auto *got_logger = get_logger();

    outer_allocator == nullptr
    ? ::operator delete(_all_memory)
    : outer_allocator->deallocate(_all_memory);

    if (got_logger != nullptr)
    {
        got_logger->log("Destructor of executed successfully", logger::severity::trace);
    }
}