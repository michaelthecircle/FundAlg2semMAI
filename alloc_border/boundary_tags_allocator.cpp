#include "boundary_tags_allocator.h"

size_t border_descriptors_memory::get_service_part_allocator_size() const
{
    return sizeof(size_t) + sizeof(logger *) + sizeof(memory::allocate_mode) + sizeof(memory *) + sizeof(void *);
}

void **border_descriptors_memory::get_first_occup_block() const
{
    return reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_all_memory) + get_service_part_allocator_size() - sizeof(void *));
}

size_t border_descriptors_memory::get_allocator_size() const
{
    return *reinterpret_cast<size_t *>(_all_memory);
}

size_t border_descriptors_memory::get_serv_occup_block() const
{
    return sizeof(size_t) + 2 * sizeof(void *);
}

void **border_descriptors_memory::get_pointer_prev_block(void *target_ptr) const
{
    return reinterpret_cast<void **>(reinterpret_cast<size_t *>(target_ptr) + 1);
}

void **border_descriptors_memory::get_pointer_next_block(void *target_ptr) const
{
    return reinterpret_cast<void **>(reinterpret_cast<size_t *>(target_ptr) + 1) + 1;
}

size_t *border_descriptors_memory::get_block_size(void *target_ptr) const
{
    return reinterpret_cast<size_t *>(target_ptr);
}

logger *border_descriptors_memory::get_logger() const noexcept
{
    return *reinterpret_cast<logger **>(reinterpret_cast<unsigned char *>(_all_memory) + sizeof(size_t));
}

std::pair<void *, size_t> border_descriptors_memory::get_aviable_block_address_and_size(
    void *const previous_occupied_block_address,
    void *const next_occupied_block_address) const
{
    size_t serv_occup_block_size = get_serv_occup_block();
    size_t allocator_size = get_allocator_size();
    size_t service_part_allocator_size = get_service_part_allocator_size();
    auto previous_occupied_block_size = previous_occupied_block_address == nullptr
                                            ? 0
                                            : *get_block_size(previous_occupied_block_address);
    size_t block_size = 0;
    void *block_address = nullptr;

    if (previous_occupied_block_address == nullptr && next_occupied_block_address == nullptr)
    {
        block_address = nullptr;
        block_size = 0;
    }
    else if (previous_occupied_block_address == nullptr)
    {
        block_address = reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(_all_memory) + service_part_allocator_size);
        block_size = reinterpret_cast<unsigned char *>(next_occupied_block_address) - reinterpret_cast<unsigned char *>(block_address);
    }
    else if (next_occupied_block_address == nullptr)
    {
        block_address = reinterpret_cast<unsigned char *>(previous_occupied_block_address) + serv_occup_block_size + previous_occupied_block_size;
        block_size = reinterpret_cast<unsigned char *>(_all_memory) + service_part_allocator_size + allocator_size - reinterpret_cast<unsigned char *>(block_address);
    }
    else
    {
        block_address = reinterpret_cast<unsigned char *>(previous_occupied_block_address) + serv_occup_block_size + previous_occupied_block_size;
        block_size = reinterpret_cast<unsigned char *>(next_occupied_block_address) - reinterpret_cast<unsigned char *>(block_address);
    }

    return {block_address, block_size};
}

std::string border_descriptors_memory::get_allocate_mode_string(allocate_mode method) const
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

std::string border_descriptors_memory::get_pointer_address(void *target_ptr) const
{

    char str[20];
    memset(str, 0, 20);
    sprintf(str, "%p", target_ptr);
    std::string add;
    add.append(str);
    return add;
}

void border_descriptors_memory::debug_alloc(const void *target_ptr) const
{
    logger *log = get_logger();
    if (log == nullptr)
    {
        return;
    }
    unsigned char *ptr = reinterpret_cast<unsigned char *>(reinterpret_cast<size_t *>(const_cast<void *>(target_ptr)) + 1) + 2 * sizeof(void *);
    size_t size = *get_block_size(const_cast<void *>(target_ptr));
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
void border_descriptors_memory::dump_allocator_state(bool is_allocate) const noexcept
{
    auto *log = get_logger();
    if (log == nullptr)
    {
        return;
    }

    auto allocator_service_part_size = get_service_part_allocator_size();

    std::string constructed_state_string("|");
    auto *current_occupied_block = *get_first_occup_block();

    if (current_occupied_block == nullptr)
    {
        constructed_state_string += "avlbl " + std::to_string(get_allocator_size()) + '|';
    }
    else
    {
        void *previous_occupied_block = nullptr;

        // if (current_occupied_block != reinterpret_cast<unsigned char *>(_all_memory) + allocator_service_part_size)
        //{
        //     auto first_available_block_size = reinterpret_cast<unsigned char *>(current_occupied_block) - reinterpret_cast<unsigned char *>(reinterpret_cast<unsigned char *>(_all_memory) + allocator_service_part_size);
        //     constructed_state_string += "avlbl " + std::to_string(first_available_block_size) + '|';
        // }

        while (true)
        {
            auto block_address_and_size = get_aviable_block_address_and_size(previous_occupied_block, current_occupied_block);
            auto aviable_block_size = block_address_and_size.second;

            if (previous_occupied_block != nullptr)
            {
                constructed_state_string +=
                    "occup " + std::to_string(*get_block_size(previous_occupied_block)) + '|';
            }

            if (aviable_block_size != 0)
            {
                constructed_state_string += "avlbl " + std::to_string(aviable_block_size) + '|';
            }

            if (current_occupied_block == nullptr)
            {
                break;
            }

            previous_occupied_block = current_occupied_block;
            current_occupied_block = *get_pointer_next_block(current_occupied_block);
        }
    }

    log->log(std::string("Memory state after ") + (is_allocate ? "" : "de") + "allocation operation: " + constructed_state_string, logger::severity::debug);
}

void *border_descriptors_memory::allocate(size_t requested_block_size) const
{
    auto alloc_method = *reinterpret_cast<memory::allocate_mode *>(reinterpret_cast<unsigned char *>(_all_memory) + sizeof(size_t) + sizeof(logger *));

    void *prev_block = nullptr;
    void *first_occup_block = *get_first_occup_block();
    void *curr_block = first_occup_block;

    void *prev_to_target_block = nullptr;
    void *target_block = nullptr;
    size_t target_block_size = 0;
    void *next_to_target_block = nullptr;

    size_t serv_occup_block_size = get_serv_occup_block();
    size_t allocator_size = get_allocator_size();
    size_t service_part_allocator_size = get_service_part_allocator_size();

    if (curr_block == nullptr && requested_block_size + serv_occup_block_size < allocator_size)
    {
        curr_block = reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(_all_memory) + service_part_allocator_size);
        *get_block_size(curr_block) = requested_block_size;
        *get_pointer_prev_block(curr_block) = nullptr;
        *get_pointer_next_block(curr_block) = nullptr;

        *get_first_occup_block() = curr_block;
        dump_allocator_state(true);
        return reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(curr_block) + serv_occup_block_size);
    }

    while (true)
    {
        auto block_address_and_size = get_aviable_block_address_and_size(prev_block, curr_block);
        auto block_address = block_address_and_size.first;
        auto block_size = block_address_and_size.second;

        if (block_size >= requested_block_size + serv_occup_block_size)
        {
            if (alloc_method == memory::allocate_mode::first_fit ||
                alloc_method == memory::allocate_mode::best_fit && (target_block == nullptr || target_block_size > block_size) ||
                alloc_method == memory::allocate_mode::worst_fit && (target_block == nullptr || target_block_size < block_size))
            {
                prev_to_target_block = prev_block;
                target_block = block_address;
                target_block_size = block_size;
                next_to_target_block = curr_block;
            }

            if (alloc_method == memory::allocate_mode::first_fit)
            {
                break;
            }
        }

        if (curr_block == nullptr)
        {
            break;
        }

        prev_block = curr_block;
        curr_block = *get_pointer_next_block(curr_block);
    }

    if (target_block == nullptr)
    {
        throw std::bad_alloc();
    }

    if (target_block_size - requested_block_size - serv_occup_block_size < serv_occup_block_size)
    {
        this->trace_with_guard("Size has changed: " + std::to_string(requested_block_size) + "->" + std::to_string(target_block_size - serv_occup_block_size));
        requested_block_size = target_block_size - serv_occup_block_size;
    }

    *reinterpret_cast<size_t *>(target_block) = requested_block_size;
    if (prev_to_target_block == nullptr)
    {
        *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_all_memory) + service_part_allocator_size - sizeof(void *)) = target_block;
        *get_pointer_prev_block(target_block) = nullptr;
    }
    else
    {
        *get_pointer_next_block(prev_to_target_block) = target_block;
        *get_pointer_prev_block(target_block) = prev_to_target_block;
    }

    *get_pointer_next_block(target_block) = next_to_target_block;

    if (next_to_target_block != nullptr)
    {
        *get_pointer_prev_block(next_to_target_block) = target_block;
    }
    this->trace_with_guard("Allocate is " + get_allocate_mode_string(alloc_method));
    dump_allocator_state(true);
    return reinterpret_cast<void *>(reinterpret_cast<void **>(reinterpret_cast<size_t *>(target_block) + 1) + 2);
}

void border_descriptors_memory::deallocate(const void *const target_to_dealloc) const
{
    auto true_target_to_dealloc = reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(const_cast<void *>(target_to_dealloc)) - get_serv_occup_block());
    debug_alloc(true_target_to_dealloc);
    auto *previous_to_target_block = *get_pointer_prev_block(const_cast<void *>(true_target_to_dealloc));
    auto *next_to_target_block = *get_pointer_next_block(const_cast<void *>(true_target_to_dealloc));
    auto const service_part_allocator_size = get_service_part_allocator_size();

    *(previous_to_target_block == nullptr
          ? get_first_occup_block()
          : get_pointer_next_block(previous_to_target_block)) = next_to_target_block;

    next_to_target_block == nullptr
        ? nullptr
        : *get_pointer_prev_block(next_to_target_block) = previous_to_target_block;

    dump_allocator_state(false);
}

border_descriptors_memory::border_descriptors_memory(logger *log, memory *allocator, size_t size, memory::allocate_mode method)
{

    auto target_size = size + get_service_part_allocator_size();
    _all_memory = reinterpret_cast<void *>(allocator != nullptr ? allocator->allocate(target_size) : ::operator new(target_size));

    auto total_size = reinterpret_cast<size_t *>(_all_memory);
    *total_size = size;
    auto log_block = reinterpret_cast<logger **>(total_size + 1);
    *log_block = log;
    auto mode_block = reinterpret_cast<memory::allocate_mode *>(log_block + 1);
    *mode_block = method;
    auto alloc_block = reinterpret_cast<memory **>(mode_block + 1);
    *alloc_block = allocator;
    auto first_occup_next_block = reinterpret_cast<void **>(alloc_block + 1);
    *first_occup_next_block = nullptr;

    this->trace_with_guard("Boundary tags system allocator initialized!");
}

border_descriptors_memory::~border_descriptors_memory()
{
    if (_all_memory == nullptr)
    {
        return;
    }
    auto *got_logger = get_logger();
    auto *outer_allocator = *reinterpret_cast<memory **>(reinterpret_cast<unsigned char *>(_all_memory) + sizeof(size_t) + sizeof(logger *) + sizeof(memory::allocate_mode));

    outer_allocator == nullptr
        ? ::operator delete(_all_memory)
        : outer_allocator->deallocate(_all_memory);

    if (got_logger != nullptr)
    {
        got_logger->log("Destructor executed successfully", logger::severity::trace);
    }
}