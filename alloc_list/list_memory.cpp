#include "list_memory.h"

list_memory::list_memory(size_t size,
                         memory::allocation_mode allocation_mode,
                         logger* logger_use,
                         memory* another_allocator)
{
    if(size < sizeof(size_t) + sizeof(void*))
    {
        throw std::logic_error("Few memory");
    }
    size_t total_size = size + sizeof(logger*) + sizeof(memory*) + sizeof(size_t) + sizeof(void*) + sizeof(memory::allocation_mode);
    try
    {
        _memory = another_allocator == nullptr
                  ? ::operator new(total_size)
                  : another_allocator->allocate(total_size);           //void* recent_mem = reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(block_pointer) - sizeof(size_t));
    }
    catch(std::bad_alloc &ex)
    {
        if(logger_use != nullptr)
        {
            logger_use->log("Allocation failed", logger::severity::error);
        }
        throw ex;
    }

    size_t *memory_size_place = reinterpret_cast<size_t *>(_memory);
    *memory_size_place = size;

    memory **outer_allocator_place = reinterpret_cast<memory**>(memory_size_place + 1);
    *outer_allocator_place = another_allocator;

    logger **logger_place = reinterpret_cast<logger**>(outer_allocator_place + 1);
    *logger_place = logger_use;

    memory::allocation_mode *mode_alloc = reinterpret_cast<memory::allocation_mode*>(logger_place + 1);
    *mode_alloc = allocation_mode;

    void** first_block = reinterpret_cast<void**>(mode_alloc + 1);
    *first_block = first_block + 1;

    *reinterpret_cast<void **>(*first_block) = nullptr;
    *reinterpret_cast<size_t*>(first_block + 2) = size - sizeof(size_t) - sizeof(void*);
}

void list_memory::destroy(){
    if(_memory == nullptr) return;
    memory* alloc = *reinterpret_cast<memory**>(_memory);
    alloc == nullptr
    ? ::operator delete(_memory)
    : alloc->deallocate(_memory);
}

list_memory::~list_memory()
{
    destroy();
}

list_memory::list_memory(list_memory&& obj)
{
    move(std::move(obj));
}//конструктор перемещения

void list_memory::move(list_memory&& obj)
{
    _memory = obj._memory;
    obj._memory = nullptr;
}

list_memory& list_memory::operator=(list_memory&& obj)
{
    destroy();
    move(std::move(obj));
    return *this;
}

logger* list_memory::get_logger() const
{
    return *reinterpret_cast<logger **>(reinterpret_cast<unsigned char *>(_memory) + sizeof(size_t) + sizeof(memory *));
}

std::string list_memory::get_typename() const noexcept
{
    return "list_heap_allocator";
}

void* const list_memory::get_first_aviable_block()const
{
    return *get_first_aviable_block_ptr();
}

void**  list_memory::get_first_aviable_block_ptr()const
{
    return reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_memory) + get_service_part_allocator_size() - sizeof(void *));
}

size_t list_memory::get_service_part_allocator_size() const
{
    return sizeof(size_t) + sizeof(memory *) + sizeof(logger *) + sizeof(allocation_mode) + sizeof(void *);
}

void* list_memory::get_right_border_of_memory_ptr() const
{
    return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(_memory) + get_service_part_allocator_size() + *reinterpret_cast<size_t*>(_memory));
}

size_t list_memory::get_current_aviable_block_size(void const* block) const
{
    return *reinterpret_cast<size_t*>(reinterpret_cast<void**>(const_cast<void*>(block)) + 1);
}

void* list_memory::get_next_aviable_block(void* block) const
{
    return *reinterpret_cast<void**>(block);
}

size_t list_memory::get_service_part_aviable_block_size()const
{
    return sizeof(void*) + sizeof(size_t);
}

size_t list_memory::get_service_part_occupied_block_size()const
{
    return sizeof(size_t);
}
size_t list_memory::get_occupied_block_size(void * const current_block_address) const
{
    return *reinterpret_cast<size_t*>(current_block_address);
}

void list_memory::log_of_changed_size() const
{
    auto *log = get_logger();
    if (log == nullptr)
    {
        return;
    }
    log->log(std::string("size has changed"), logger::severity::debug);
}

void* list_memory::allocate(size_t block_size) const{
    auto overridden_block_size = block_size;
    if (overridden_block_size < sizeof(void*))
    {
        overridden_block_size = sizeof(void*);
    }
    memory::allocation_mode current_alloc_mode = *reinterpret_cast<memory::allocation_mode*>(reinterpret_cast<unsigned char*>(_memory)+sizeof(size_t)+sizeof(memory*)+sizeof(logger*));
    void* current_aviable_block = get_first_aviable_block();
    void* prev_block = nullptr;
    void* target_block = nullptr,
            *prev_to_target_block = nullptr,
            *next_to_target_block = nullptr;

    while (current_aviable_block != nullptr)
    {
        void* next_block = get_next_aviable_block(current_aviable_block);
        auto curr_sz = get_current_aviable_block_size(current_aviable_block);
        if (curr_sz + sizeof(void*) >= overridden_block_size)
        {
            if (current_alloc_mode == memory::allocation_mode::first_fit ||
                current_alloc_mode == memory::allocation_mode::best_fit && (target_block == nullptr || curr_sz < get_current_aviable_block_size(target_block)) ||
                current_alloc_mode == memory::allocation_mode::worst_fit && (target_block == nullptr || curr_sz > get_current_aviable_block_size(target_block)))
            {
                target_block = current_aviable_block;
                prev_to_target_block = prev_block;
                next_to_target_block = next_block;
            }

            if (current_alloc_mode == memory::allocation_mode::first_fit)
            {
                break;
            }
        }

        prev_block = current_aviable_block;
        current_aviable_block = next_block;
    }

    if (target_block == nullptr)
    {
        throw std::bad_alloc();
    }

    auto aviable_block_service_part_size = get_service_part_aviable_block_size();
    auto occupied_block_service_part_size = get_service_part_occupied_block_size();
    size_t block_without_size_info = get_current_aviable_block_size(target_block);

    if (block_without_size_info + aviable_block_service_part_size - (overridden_block_size + sizeof(size_t)) < aviable_block_service_part_size)
    {
        overridden_block_size = block_without_size_info + sizeof(void *);
    }

    if (overridden_block_size != block_size)
    {
        list_memory::log_of_changed_size();
        block_size = overridden_block_size;
    }

    void* next_for_prev;
    if (get_current_aviable_block_size(target_block) + get_service_part_aviable_block_size() == block_size + occupied_block_service_part_size)
    {
        next_for_prev = next_to_target_block;
    }
    else
    {
        next_for_prev = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(target_block) + block_size + occupied_block_service_part_size);
        *reinterpret_cast<void**>(next_for_prev) = next_to_target_block;
        *(reinterpret_cast<size_t *>(reinterpret_cast<void**>(next_for_prev) + 1)) = block_without_size_info - block_size - sizeof(size_t);
    }
    if(prev_to_target_block == nullptr)
    {
        auto start_ptr = get_first_aviable_block_ptr();
        *start_ptr = next_for_prev;
    }
    else{
        *reinterpret_cast<void**>(prev_to_target_block) = next_for_prev;
    }
    *reinterpret_cast<size_t *>(target_block) = block_size;
    dump_allocator_state(true);
    return reinterpret_cast<void *>(reinterpret_cast<size_t *>(target_block) + 1);
}

void list_memory::deallocate(void * const block_pointer)const
{
    void* true_block_pointer = reinterpret_cast<void*>(reinterpret_cast<size_t*>(const_cast<void*>(block_pointer)) - 1);
    auto current_block = get_first_aviable_block();

    if ((true_block_pointer < reinterpret_cast<unsigned char *>(_memory) + get_service_part_allocator_size()) || (true_block_pointer >= reinterpret_cast<unsigned char *>(get_right_border_of_memory_ptr()) - get_service_part_occupied_block_size() - sizeof(void *)))
    {
        throw memory::dealloc_fail();
    }
    void* prev_block = nullptr;

    while (current_block < block_pointer)
    {
        prev_block = current_block;
        current_block = get_next_aviable_block(current_block);
    }

    auto block_to_deallocate_size = *reinterpret_cast<size_t *>(true_block_pointer);
    if (prev_block == nullptr && current_block == nullptr)
    {
        *reinterpret_cast<void**>(true_block_pointer) = nullptr;
        block_to_deallocate_size = *reinterpret_cast<size_t*>(reinterpret_cast<void**>(true_block_pointer) + 1) = block_to_deallocate_size - sizeof(void*);
        *get_first_aviable_block_ptr() = true_block_pointer;
    }
    else
    {
        if (current_block == nullptr)
        {
            *reinterpret_cast<void **>(true_block_pointer) = nullptr;
            block_to_deallocate_size = *reinterpret_cast<size_t *>(reinterpret_cast<void **>(true_block_pointer) + 1) = block_to_deallocate_size - sizeof(void *);
        }
        else
        {
            if (reinterpret_cast<unsigned char *>(block_pointer) + block_to_deallocate_size == current_block)
            {
                *reinterpret_cast<void**>(true_block_pointer) = get_next_aviable_block(current_block);
                block_to_deallocate_size = *reinterpret_cast<size_t *>(reinterpret_cast<void **>(true_block_pointer) + 1) = sizeof(size_t) + block_to_deallocate_size + get_current_aviable_block_size(current_block);
                current_block = true_block_pointer;
            }
            else
            {
                *reinterpret_cast<void **>(true_block_pointer) = current_block;
                block_to_deallocate_size = *reinterpret_cast<size_t *>(reinterpret_cast<void **>(true_block_pointer) + 1) = block_to_deallocate_size - sizeof(void *);
            }
        }

        if (prev_block == nullptr)
        {
            *get_first_aviable_block_ptr() = true_block_pointer;
        }
        else
        {
            auto prev_block_size = get_current_aviable_block_size(prev_block);
            if (reinterpret_cast<unsigned char *>(prev_block) + get_service_part_aviable_block_size() + prev_block_size == true_block_pointer)
            {
                *reinterpret_cast<void **>(prev_block) = get_next_aviable_block(true_block_pointer);
                *reinterpret_cast<size_t *>(reinterpret_cast<void **>(prev_block) + 1) = prev_block_size + block_to_deallocate_size + get_service_part_aviable_block_size();
            }
            else
            {
                *reinterpret_cast<void **>(prev_block) = true_block_pointer;
            }
        }
    }

    dump_allocator_state(false);
}

void list_memory::dump_allocator_state(bool is_allocate) const noexcept
{
    auto *log = get_logger();
    if (log == nullptr)
    {
        return;
    }

    auto allocator_memory_size = *reinterpret_cast<size_t *>(_memory);
    std::string constructed_state_string("|");
    auto *current_aviable_block = get_first_aviable_block();
    auto *current_any_block = reinterpret_cast<unsigned char *>(_memory) + sizeof(size_t) + sizeof(memory*) + sizeof(logger *) + sizeof(allocation_mode) + sizeof(void *);

    while (current_any_block - reinterpret_cast<unsigned char *>(_memory) < allocator_memory_size)
    {
        size_t any_block_size;
        if (current_any_block == current_aviable_block)
        {
            constructed_state_string += "avlbl " + std::to_string(any_block_size = get_current_aviable_block_size(current_any_block)) + '|';
            current_aviable_block = get_next_aviable_block(current_aviable_block);
            any_block_size += get_service_part_aviable_block_size();
        }
        else
        {
            constructed_state_string += "occpd " + std::to_string(any_block_size = get_occupied_block_size(current_any_block)) + '|';
            any_block_size += get_service_part_occupied_block_size();
        }
        current_any_block += any_block_size;
    }
    log->log(std::string("Memory state after ") + (is_allocate? "": "de") + "allocation operation: " + constructed_state_string, logger::severity::debug);
}






/*if (current_pointer < reinterpret_cast<void*>(size_of_deallocated_block))
        {
            //возьмем слева свободный участок
            size_t size_of_current_available_block = get_current_aviable_block_size(current_pointer);
            if ((reinterpret_cast<unsigned char*>(current_pointer) + size_of_current_available_block) == reinterpret_cast<void*>(size_of_deallocated_block)) //если ближайший участок слева свободный
            {
                if (*reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block_pointer) + size_of_deallocated_block - sizeof(void*)) = nullptr) //если свободный справа последний
                {
                    size_t* length_of_left_available_ptr = reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(current_pointer) - sizeof(void*));
                    size_t* length_of_right_available_ptr = reinterpret_cast<size_t *>(reinterpret_cast<unsigned char*>(block_pointer) + size_of_deallocated_block - 2*sizeof(void*));

                }
                else {
                    size_t* distance_to_next_available_ptr = reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(current_pointer) - sizeof(void*));
                    *distance_to_next_available_ptr += size_of_deallocated_block;
                }
            }
            else
            {

            }
        }*/


/*
if (right_block != nullptr)
{
void* right_right_block = get_next_aviable_block(right_block); //может быть nullptr
size_t size_of_right_block = get_current_aviable_block_size(right_block);
int dealloc_case = 0;
void** ptr_from_left_to_available_block = nullptr;
void** new_pointer_to_next_available = nullptr;
size_t* new_ptr_of_size_available = nullptr;
size_t size_of_new_available_block = 0;
void** place_for_next_available_block = nullptr;
size_t* new_available_block_size = nullptr;
void* new_available_block = nullptr;
void** ptr_from_left_to_available = nullptr;
size_t* size_of_left_block = nullptr;
void** next_new_available_block = nullptr;
if (right_block == (reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(reinterpret_cast<size_t*>(block_pointer) + 1) + deallocated_block_size))) //проверка является ли ближайший справа блок свободным
{
dealloc_case = 1; //блок справа свободен
}
if ((reinterpret_cast<unsigned char*>(current_block) + sizeof(size_t) + sizeof(void*) + get_current_aviable_block_size(current_block)) == block_pointer) //аналогичная проверка на свободный блок слева
{
dealloc_case += 2;
}
switch (dealloc_case) {
case 0: // и справа и слева занятые
ptr_from_left_to_available_block = reinterpret_cast<void**>(current_block);
size_of_new_available_block = deallocated_block_size + get_service_part_occupied_block_size();
new_pointer_to_next_available = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block_pointer) - sizeof(size_t));
*new_pointer_to_next_available = right_block;
new_ptr_of_size_available = reinterpret_cast<size_t*>(new_pointer_to_next_available + 1);
*new_ptr_of_size_available = size_of_new_available_block - get_service_part_aviable_block_size();
*ptr_from_left_to_available_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(new_ptr_of_size_available) + sizeof(size_t));
break;
case 1: // справа свободен
deallocated_block_size += size_of_right_block + get_service_part_aviable_block_size() + get_service_part_occupied_block_size();
place_for_next_available_block = reinterpret_cast<void**>(block_pointer);
*place_for_next_available_block = get_next_aviable_block(right_block);
new_available_block_size = reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(*place_for_next_available_block) + sizeof(void*));
*new_available_block_size = deallocated_block_size;
new_available_block = reinterpret_cast<void*>(new_available_block_size + 1);
ptr_from_left_to_available = reinterpret_cast<void**>(current_block);
*ptr_from_left_to_available = new_available_block;
break;
case 2: // слева свободен
*reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(current_block) + sizeof(void*)) += deallocated_block_size + get_service_part_occupied_block_size();
break;
case 3: // и справа и слева свободные
deallocated_block_size += size_of_right_block + get_service_part_aviable_block_size();
size_of_left_block = reinterpret_cast<size_t*>(get_current_aviable_block_size(current_block));
(*size_of_left_block) += deallocated_block_size + get_service_part_occupied_block_size();
next_new_available_block = reinterpret_cast<void**>(get_next_aviable_block(current_block));
(*next_new_available_block) = right_right_block;
break;
}
}
else
{
*reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(current_block) + sizeof(void*)) += deallocated_block_size + get_service_part_occupied_block_size();
}
*/