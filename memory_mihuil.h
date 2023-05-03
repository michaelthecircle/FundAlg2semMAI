#ifndef SANDBOX_CPP_MEMORY_MIHUIL_H
#define SANDBOX_CPP_MEMORY_MIHUIL_H

#include "memory.h"
#include "logger.h"
#include "logger_holder.h"
#include "typename_holder.h"

class memory_mihuil final:
    public memory,
    protected logger_holder,
    protected typename_holder
{

private:

    logger *_logger;

public:

    explicit memory_mihuil(
        logger *logger = nullptr);

public:

    size_t get_occupied_block_size(
        void * const current_block_address) const override;

    void * const allocate(
        size_t block_size)override;

    void deallocate(
        void * const block_pointer) const override;

    void setup_allocation_mode(
        memory::allocation_mode mode) override;

private:

    logger *get_logger() const override;

private:

    std::string get_typename() const noexcept override;

};

#endif //SANDBOX_CPP_MEMORY_MIHUIL_H
