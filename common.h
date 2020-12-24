#pragma once

#include <avr/io.h>

// Register helpers

#define set_bit(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define get_bit(sfr, bit) (!!bit_is_set(sfr, bit))
#define clear_bit(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define toggle_bit(sfr, bit) (_SFR_BYTE(sfr) ^= _BV(bit))


// Math

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
