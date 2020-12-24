#include <avr/io.h>
#include <util/delay.h>

#include "serial.h"


static void initialize(void);


int main(void) {
  initialize();

  while (1) {
    serial_printf(&serial0, "Hello, world\r\n");
    _delay_ms(100);
  }

  return 0;
}


static void initialize(void) {
  serial_init(&serial0, 115200);
}
