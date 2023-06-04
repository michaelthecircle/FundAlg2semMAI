#ifndef SANDBOX_CPP_MEMORY_WITH_FIT_ALLOCATION_H
#define SANDBOX_CPP_MEMORY_WITH_FIT_ALLOCATION_H

#include "memory.h"

class memory_with_fit_allocation:
    public memory
{

public:

    enum class allocation_mode
    {
        first_fit,
        the_best_fit,
        the_worst_fit
    };

public:

    memory_with_fit_allocation(
        memory_with_fit_allocation const &other) = delete;

    memory_with_fit_allocation &operator=(
        memory_with_fit_allocation const &other) = delete;

protected:

    memory_with_fit_allocation() = default;

protected:

    virtual allocation_mode get_allocation_mode() const = 0;

public:

    virtual void setup_allocation_mode(
        allocation_mode mode) = 0;

};

#endif //SANDBOX_CPP_MEMORY_WITH_FIT_ALLOCATION_H
