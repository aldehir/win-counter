#include <stdio.h>

#include "serial.h"
#include "common.h"


serial_t serial0 = {&UCSR0A, &UCSR0B, &UCSR0C, &UBRR0H, &UBRR0L, &UDR0};


void serial_init(serial_t *serial, uint32_t baud) {
  uint16_t ubrr = (F_CPU / 8 / baud - 1);
  *serial->ubrrh = ubrr >> 8;
  *serial->ubrrl = ubrr;

  set_bit(*serial->ucsra, U2X0);
  set_bit(*serial->ucsrb, TXEN0);
  *serial->ucsrc = (3 << UCSZ00);
}


static void serial_send_byte(serial_t *serial, char b) {
  loop_until_bit_is_set(*serial->ucsra, UDRE0);
  *serial->udr = b;
}


void serial_send(serial_t *serial, const char *data, const size_t size) {
  const char *end = data + size;

  while (data != end) {
    serial_send_byte(serial, *data);
    data++;
  }
}


void serial_printf(serial_t *serial, const char *fmt, ...) {
  char buffer[SERIAL_PRINTF_BUFFER_SIZE];

  va_list args;
  va_start(args, fmt);
  int size = vsnprintf(buffer, SERIAL_PRINTF_BUFFER_SIZE, fmt, args);
  va_end(args);

  serial_send(serial, buffer, MIN(size, SERIAL_PRINTF_BUFFER_SIZE));
}
