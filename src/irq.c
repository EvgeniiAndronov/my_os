#include "irq.h"
#include "isr.h"
#include "screen.h"
#include "pic.h"

static irq_handler_t irq_handlers[16] = {0};

void irq_handler(struct regs *r) {
    // Упрощенная диагностика
    kprint("IRQ received: ");
    char msg[10];
    int_to_str(r->int_no, msg);
    kprint(msg);
    kprint("\n");

    // Номер IRQ должен быть 0-15
    uint8_t irq_num = r->int_no;

    if (irq_num < 16 && irq_handlers[irq_num]) {
        kprint("Calling handler for IRQ");
        kprint(msg);
        kprint("\n");
        irq_handlers[irq_num]();
    } else {
        kprint("No handler for IRQ");
        kprint(msg);
        kprint("\n");
    }

    // Отправляем EOI
    pic_send_eoi(irq_num);
}

void irq_install_handler(int irq, irq_handler_t handler) {
    if (irq < 16) {
        kprint("Installing handler for IRQ");
        char msg[10];
        int_to_str(irq, msg);
        kprint(msg);
        kprint("\n");
        irq_handlers[irq] = handler;
    } else {
        kprint("Attempt to install handler for invalid IRQ");
        char msg[10];
        int_to_str(irq, msg);
        kprint(msg);
        kprint("\n");
    }
}

void irq_install() {
    // Инициализация будет позже
}