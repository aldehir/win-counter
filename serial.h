#pragma once

#include <stddef.h>
#include <stdint.h>


#if !defined(SERIAL_PRINTF_BUFFER_SIZE)
#define SERIAL_PRINTF_BUFFER_SIZE 128
#endif


typedef struct {
  volatile uint8_t *ucsra;
  volatile uint8_t *ucsrb;
  volatile uint8_t *ucsrc;

  volatile uint8_t *ubrrh;
  volatile uint8_t *ubrrl;

  volatile uint8_t *udr;
} serial_t;

extern serial_t serial0;

void serial_init(serial_t *serial, uint32_t baud);
void serial_send(serial_t *serial, const char *data, const size_t size);
void serial_printf(serial_t *serial, const char *fmt, ...);
