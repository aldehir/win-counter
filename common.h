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
} pin_config_t;


#define D0 ((pin_config_t){ &DDRD, &PORTD, &PIND, 0 })
#define D1 ((pin_config_t){ &DDRD, &PORTD, &PIND, 1 })
#define D2 ((pin_config_t){ &DDRD, &PORTD, &PIND, 2 })
#define D3 ((pin_config_t){ &DDRD, &PORTD, &PIND, 3 })
#define D4 ((pin_config_t){ &DDRD, &PORTD, &PIND, 4 })
#define D5 ((pin_config_t){ &DDRD, &PORTD, &PIND, 5 })
#define D6 ((pin_config_t){ &DDRD, &PORTD, &PIND, 6 })
#define D7 ((pin_config_t){ &DDRD, &PORTD, &PIND, 7 })

#define B0 ((pin_config_t){ &DDRB, &PORTB, &PINB, 0 })
#define B1 ((pin_config_t){ &DDRB, &PORTB, &PINB, 1 })
#define B2 ((pin_config_t){ &DDRB, &PORTB, &PINB, 2 })
#define B3 ((pin_config_t){ &DDRB, &PORTB, &PINB, 3 })
#define B4 ((pin_config_t){ &DDRB, &PORTB, &PINB, 4 })
#define B5 ((pin_config_t){ &DDRB, &PORTB, &PINB, 5 })
#define B6 ((pin_config_t){ &DDRB, &PORTB, &PINB, 6 })
#define B7 ((pin_config_t){ &DDRB, &PORTB, &PINB, 7 })

#define set_pin_output(pin) set_bit(*pin.ddr, pin.bit)
#define set_pin_input(pin) clear_bit(*pin.ddr, pin.bit)

#define read_pin(pin) get_bit(*pin.pin, pin.bit)
#define write_pin_high(pin) set_bit(*pin.port, pin.bit)
#define write_pin_low(pin) clear_bit(*pin.port, pin.bit)
#define write_pin(pin, value) assign_bit(*pin.port, pin.bit, value)
#define toggle_pin(pin) toggle_bit(*pin.port, pin.bit)
