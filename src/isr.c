#include "isr.h"
#include "screen.h"

void isr_handler(struct regs *r) {
    char msg[40];
    kprint("Interrupt received: ");
    int_to_str(r->int_no, msg);
    kprint(msg);
    kprint("\n");

    if (r->int_no < 32) {
        kprint("System halted!\n");
        for(;;);
    }
}