#include "pic.h"
#include "ports.h"
#include "screen.h"

void pic_remap() {
    // Сохраняем текущие маски
    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);

    // Посылаем команду инициализации
    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);

    // Устанавливаем смещения векторов прерываний
    outb(PIC1_DATA, 0x20); // IRQ 0-7 -> INT 32-39
    outb(PIC2_DATA, 0x28); // IRQ 8-15 -> INT 40-47

    // Настраиваем связь между контроллерами
    outb(PIC1_DATA, 0x04); // Ведущий PIC: ведомый подключен к IRQ2
    outb(PIC2_DATA, 0x02); // Ведомый PIC: identity

    // Устанавливаем режим работы
    outb(PIC1_DATA, 0x01); // Режим 8086/8088
    outb(PIC2_DATA, 0x01);

    // Восстанавливаем сохраненные маски
    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

void pic_send_eoi(uint8_t irq) {
    // Проверка допустимого диапазона IRQ
    if (irq > 15) {
        kprint("Invalid IRQ in pic_send_eoi: ");
        char msg[10];
        int_to_str(irq, msg);
        kprint(msg);
        kprint("\n");
        return;
    }

    // Отправляем EOI
    if (irq >= 8) {
        outb(PIC2_CMD, 0x20);
    }
    outb(PIC1_CMD, 0x20);
}