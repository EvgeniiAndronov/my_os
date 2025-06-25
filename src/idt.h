#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_ENTRIES 256

// Структура записи IDT
struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

// Структура указателя на IDT (для lidt)
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Исключения процессора
#define IRQ0 32
#define IRQ1 33

void init_idt();
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);

#endif