#ifndef REGS_H
#define REGS_H

#include <stdint.h>

struct regs {
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;   // ESP на момент pusha (не оригинальный)
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t int_no;
    uint32_t err_code;
} __attribute__((packed));

#endif