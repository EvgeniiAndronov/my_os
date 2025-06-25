#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

void kprint(const char *str);
void clear_screen();
void init_screen();
void int_to_str(uint32_t num, char *str);
void print_hex(uint32_t n);

#endif