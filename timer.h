#pragma once

#include <stdint.h>


typedef uint32_t timer_t;

void timer_init();
timer_t timer_read();
timer_t timer_elapsed(const timer_t ref);
