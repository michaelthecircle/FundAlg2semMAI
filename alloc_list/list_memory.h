#include "./memory.h"
#include "./logger/logger.h"
#include "./logger/logger_holder.h"
#include <cstddef>
#include "./typename_holder.h"
#include "../not_implemented.h"

class list_memory final: public memory, private typename_holder, private logger_holder
{
public:
    void * allocate(size_t block_size) const override;
    void deallocate(void * const block_pointer) const override;
private:
    size_t get_occupied_block_size(void * const current_block_address) const override;
    void* const get_first_aviable_block()const override;
    void** get_first_aviable_block_ptr()const override;
    void* get_right_border_of_memory_ptr() const override;
    size_t get_current_aviable_block_size(void const*) const override;
    void* get_next_aviable_block(void*) const  override;
    size_t get_service_part_aviable_block_size()const override;
    size_t get_service_part_occupied_block_size()const override;
    logger* get_logger() const override;
    std::string get_typename() const noexcept override;
    void dump_allocator_state(bool is_allocate) const noexcept;
    size_t get_service_part_allocator_size() const override;
    void log_of_changed_size() const override;
private:
    void* _memory;
public:
    list_memory(size_t size,
                memory::allocation_mode allocation_mode,
                logger* logger = nullptr,
                memory* another_allocator = nullptr); // конструктор
     ~list_memory(); //деструктор
    list_memory(list_memory const& obj) = delete; // копирование
    list_memory& operator=(list_memory const& obj) = delete; //присваивание
    explicit list_memory(list_memory&&); //конструктор перемещения
    list_memory& operator=(list_memory&& obj); //присваивание перемещением
private:
    void destroy();
    void move(list_memory&&);
};

