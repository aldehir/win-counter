#include "common.h"
#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>


#define CTC_MATCH ((F_CPU / 1000) / 8)

timer_t g_timer = 0UL;


ISR(TIMER1_COMPA_vect) {
  g_timer++;
}


void timer_init() {
  set_bit(TCCR1B, WGM12);
  set_bit(TCCR1B, CS11);

  OCR1AH = CTC_MATCH >> 8;
  OCR1AL = CTC_MATCH & 0xFF;

  set_bit(TIMSK1, OCIE1A);

  sei();
}

uint32_t timer_read() {
  uint32_t value;

  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    value = g_timer;
  }

  return value;
}


uint32_t timer_elapsed(const timer_t ref) {
  return timer_read() - ref;
}
