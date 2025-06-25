#include "idt.h"
#include "screen.h" // Для отладки
#include "pic.h"

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

extern void idt_load(); // Ассемблерная функция
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// Установка обработчика в IDT
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

// Инициализация IDT
void init_idt() {
    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base = (uint32_t)&idt;

    // Обнуляем всю IDT
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Перенастраиваем PIC
    pic_remap();

    // Устанавливаем обработчики для IRQ
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E); // IRQ0
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E); // IRQ1
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E); // IRQ2
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E); // IRQ3
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E); // IRQ4
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E); // IRQ5
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E); // IRQ6
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E); // IRQ7
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E); // IRQ8
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E); // IRQ9
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E); // IRQ10
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E); // IRQ11
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E); // IRQ12
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E); // IRQ13
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E); // IRQ14
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E); // IRQ15

    idt_load();
}