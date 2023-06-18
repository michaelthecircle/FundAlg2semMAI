#ifndef _MEMORY
#define _MEMORY
#include <stddef.h>
#include <iostream>
#include "./logger/logger.h"

class memory
{

public:
    enum class allocate_mode
    {
        first_fit,
        best_fit,
        worst_fit
    };

public:
    class memory_exception final : public std::exception
    {
    private:
        std::string _msg;

    public:
        const char *what() const noexcept override;

        memory_exception(const std::string &msg);
    };

public:
    virtual ~memory() noexcept = default;

    memory(
            memory const &other) = delete;

    explicit memory(
            memory &&other) = delete;

    memory &operator=(
            memory const &other) = delete;

    memory &operator=(
            memory &&other) = delete;

protected:
    memory() = default;

protected:
    // virtual size_t get_trusted_memory_size() const = 0;

    // virtual size_t get_allocator_service_block_size() const = 0;

    // virtual size_t get_available_block_service_block_size() const = 0;

    // virtual size_t get_occupied_block_service_block_size() const = 0;

    // virtual bool get_block_occupancy(
    //     void const *block_pointer) const = 0;

    // virtual void **get_first_available_block_address_address() const = 0;

    // virtual void *get_first_available_block_address() const = 0;

    // virtual void **get_first_occupied_block_address_address() const = 0;

    // virtual void *get_first_occupied_block_address() const = 0;

    // virtual size_t get_available_block_size(
    //     void const *current_block_address) const = 0;

    // virtual size_t get_occupied_block_size(
    //     void const *current_block_address) const = 0;

    // virtual void *get_available_block_previous_available_block_address(
    //     void const *current_block_address) const = 0;

    // virtual void *get_available_block_next_available_block_address(
    //     void const *current_block_address) const = 0;

    // virtual void *get_occupied_block_previous_occupied_block_address(
    //     void const *current_block_address) const = 0;

    // virtual void *get_occupied_block_next_occupied_block_address(
    //     void const *current_block_address) const = 0;

    // virtual void *get_available_block_previous_block_address(
    //     void const *current_block_address) const = 0;

    // virtual void *get_available_block_next_block_address(
    //     void const *current_block_address) const = 0;

    // virtual void *get_occupied_block_previous_block_address(
    //     void const *current_block_address) const = 0;

    // virtual void *get_occupied_block_next_block_address(
    //     void const *current_block_address) const = 0;

    // virtual void dump_occupied_block_before_deallocate(
    //     void *current_block_address) const = 0;

    // virtual void dump_trusted_memory_blocks_state() const = 0;

    virtual void *get_pointer_block(void *target_ptr) const;

    virtual size_t get_size_block(void *target_ptr) const;

    virtual void *get_first_avail_block() const;

    virtual void **get_first_avail_block_ptr() const;

    virtual size_t get_service_part_allocator_size() const;

    virtual size_t get_service_part_avail_block_size() const;

    virtual logger *get_logger() const;

    virtual void dump_allocator_state(bool is_allocate) const;

    virtual void *get_right_border_of_memory_ptr() const;

    virtual std::string get_pointer_address(void *target_ptr) const;

    virtual void debug_alloc(const void *target_ptr) const;

    virtual std::string get_allocate_mode_string(allocate_mode method) const;

    virtual void **get_first_occup_block() const;

    virtual size_t get_allocator_size() const;

    virtual size_t get_occup_block_serv_size() const;

    virtual size_t get_avail_block_serv_size() const;

    virtual void **get_pointer_prev_block(void *target_ptr) const;

    virtual void **get_pointer_next_block(void *target_ptr) const;

    virtual std::pair<void *, size_t> get_aviable_block_address_and_size(
            void *const previous_occupied_block_address,
            void *const next_occupied_block_address) const;

    virtual size_t get_block_avail_size(void *target_ptr) const;

    virtual void *get_pointer_avail_prev_block(void *target_ptr) const;

    virtual void *get_pointer_avail_next_block(void *target_ptr) const;

    virtual unsigned char compact_block_size_and_block_occup(size_t size_as_power_of_2, bool block_occup) const;

    virtual void *get_buddies(void *target_ptr) const = 0;

    virtual bool is_free_block(void *target_ptr) const = 0;

    virtual memory *get_memory() const = 0;

public:
    virtual void *allocate(
            size_t requested_block_size) const = 0;

    virtual void deallocate(
            void *block_to_deallocate_address) const = 0;

    // public:
    //     void *operator+=(
    //         size_t requested_block_size);

    //     void operator-=(
    //         void *block_to_deallocate_address);

protected:
    static std::string address_to_hex(
            void const *pointer) noexcept;
};

//for memory_mihuil changed get_service and log_of_changed_size to virtual from pure virtual
#endif //_MEMORY
