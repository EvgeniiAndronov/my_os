#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_install();
void timer_wait(uint32_t ticks);

#endif