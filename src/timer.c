#include "timer.h"
#include "irq.h"
#include "ports.h"
#include "screen.h"

#define TIMER_FREQ 1193180
#define TIMER_CMD 0x43
#define TIMER_DATA 0x40

static uint32_t tick = 0;

static void timer_handler() {
    tick++;
}

void timer_install() {
    // Регистрируем обработчик прерывания таймера
    kprint("Registering timer handler for IRQ0\n");
    irq_install_handler(0, timer_handler);

    // Устанавливаем частоту таймера (100 Гц)
    uint32_t divisor = TIMER_FREQ / 100;
    outb(TIMER_CMD, 0x36);
    outb(TIMER_DATA, divisor & 0xFF);
    outb(TIMER_DATA, (divisor >> 8) & 0xFF);

    kprint("Timer initialized at 100Hz\n");
}

void timer_wait(uint32_t ticks) {
    uint32_t end = tick + ticks;

    kprint("Start tick: ");
    char start_str[10];
    int_to_str(tick, start_str);
    kprint(start_str);
    kprint("\nEnd tick: ");
    char end_str[10];
    int_to_str(end, end_str);
    kprint(end_str);
    kprint("\n");

    while (tick < end) {
        asm volatile("hlt");
    }

    kprint("Wait completed at tick: ");
    char current_str[10];
    int_to_str(tick, current_str);
    kprint(current_str);
    kprint("\n");
}