#pragma once

#include <avr/io.h>

// Register helpers

#define set_bit(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define get_bit(sfr, bit) (!!bit_is_set(sfr, bit))
#define clear_bit(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define toggle_bit(sfr, bit) (_SFR_BYTE(sfr) ^= _BV(bit))
#define assign_bit(sfr, bit, v) (_SFR_BYTE(sfr) = (_SFR_BYTE(sfr) & ~_BV(bit)) | (!!(v) << bit))


// Math

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))


// Pin helpers

typedef struct {
  volatile uint8_t *ddr;
  volatile uint8_t *port;
  volatile uint8_t *pin;
  uint8_t bit;

  const char *comment;
} pin_config_t;


#define D0 ((pin_config_t){ &DDRD, &PORTD, &PIND, 0, "PD0 (D0)" })
#define D1 ((pin_config_t){ &DDRD, &PORTD, &PIND, 1, "PD1 (D1)" })
#define D2 ((pin_config_t){ &DDRD, &PORTD, &PIND, 2, "PD2 (D2)" })
#define D3 ((pin_config_t){ &DDRD, &PORTD, &PIND, 3, "PD3 (D3)" })
#define D4 ((pin_config_t){ &DDRD, &PORTD, &PIND, 4, "PD4 (D4)" })
#define D5 ((pin_config_t){ &DDRD, &PORTD, &PIND, 5, "PD5 (D5)" })
#define D6 ((pin_config_t){ &DDRD, &PORTD, &PIND, 6, "PD6 (D6)" })
#define D7 ((pin_config_t){ &DDRD, &PORTD, &PIND, 7, "PD7 (D7)" })

#define B0 ((pin_config_t){ &DDRB, &PORTB, &PINB, 0, "PB0 (D8)" })
#define B1 ((pin_config_t){ &DDRB, &PORTB, &PINB, 1, "PB1 (D9)" })
#define B2 ((pin_config_t){ &DDRB, &PORTB, &PINB, 2, "PB2 (D10)" })
#define B3 ((pin_config_t){ &DDRB, &PORTB, &PINB, 3, "PB3 (D11)" })
#define B4 ((pin_config_t){ &DDRB, &PORTB, &PINB, 4, "PB4 (D12)" })
#define B5 ((pin_config_t){ &DDRB, &PORTB, &PINB, 5, "PB5 (D13)" })
#define B6 ((pin_config_t){ &DDRB, &PORTB, &PINB, 6, "PB6 (XTAL1)" })
#define B7 ((pin_config_t){ &DDRB, &PORTB, &PINB, 7, "PB7 (XTAL2)" })

#define set_pin_output(p) set_bit(*p.ddr, p.bit)
#define set_pin_input(p) clear_bit(*p.ddr, p.bit)

#define read_pin(p) get_bit(*p.pin, p.bit)
#define write_pin_high(p) set_bit(*p.port, p.bit)
#define write_pin_low(p) clear_bit(*p.port, p.bit)
#define write_pin(p, value) assign_bit(*p.port, p.bit, value)
#define toggle_pin(p) toggle_bit(*p.port, p.bit)
