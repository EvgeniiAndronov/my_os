#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "pic.h"
#include "timer.h"
#include "screen.h"

void kmain() {
    init_gdt();
    kprint("GDT initialized\n");

    init_idt();
    kprint("IDT initialized\n");

    init_screen();
    kprint("Screen initialized\n");

    pic_remap();
    kprint("PIC remapped\n");

    irq_install();
    kprint("IRQ installed\n");

    timer_install();
    kprint("Timer installed\n");

    // Разрешаем прерывания
    asm volatile("sti");
    kprint("Interrupts enabled!\n");

    // Демонстрация работы таймера
    kprint("Waiting for 3 seconds...\n");
    timer_wait(300);
    kprint("Done waiting!\n");

    while(1) {
        asm volatile("hlt");
    }
}