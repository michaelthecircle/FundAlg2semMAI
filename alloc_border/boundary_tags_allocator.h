#ifndef BORDER_DESCRIPTORS_MEMORY_H
#define BORDER_DESCRIPTORS_MEMORY_H

#include <iostream>
#include "./logger/logger.h"
#include "./logger/logger_concrete.h"
#include "./logger/logger_holder.h"
#include <cstring>
#include "./memory.h"

class border_descriptors_memory final : public memory, private logger_holder
{
private:
    void *_all_memory;

protected:
    size_t get_service_part_allocator_size() const override;
    void **get_first_occup_block() const override;
    size_t get_allocator_size() const override;
    size_t get_occup_block_serv_size() const override;
    void **get_pointer_prev_block(void *target_ptr) const override;
    void **get_pointer_next_block(void *target_ptr) const override;
    size_t get_size_block(void *target_ptr) const override;
    void dump_allocator_state(bool is_allocate) const noexcept override;
    logger *get_logger() const noexcept override;
    void debug_alloc(const void *target_ptr) const override;
    std::pair<void *, size_t> get_aviable_block_address_and_size(
            void *const previous_occupied_block_address,
            void *const next_occupied_block_address) const override;
    std::string get_pointer_address(void *target_ptr) const override;
    std::string get_allocate_mode_string(allocate_mode method) const override;
    memory *get_memory() const override;
    void *get_buddies(void *target_ptr) const override;
    bool is_free_block(void *target_ptr) const override;

public:
    void *allocate(size_t requested_block_size) const override;

public:
    void deallocate(void *block_to_deallocate_address) const override;

public:
    border_descriptors_memory(logger *log, memory *allocator, size_t size, memory::allocate_mode method);

public:
    border_descriptors_memory(border_descriptors_memory const &) = delete;
    border_descriptors_memory &operator=(border_descriptors_memory const &) = delete;

public:
    ~border_descriptors_memory();
};

#endif // BORDER_DESCRIPTORS_MEMORY_H