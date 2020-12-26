#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>

#include "serial.h"
#include "debug.h"
#include "ili9340.h"


ili9340_t tft = {&spi, B2, D6, D5};


static void initialize(void) {
  serial_init(&serial0, 115200);
  spi_init(&spi);
  ili9340_init(&tft);
}


int main(void) {
  initialize();

  ili9340_memory_write(&tft);
  ili9340_fill(&tft, (ili9340_color_t){ 0x3f, 0, 0 }, 25600);
  ili9340_fill(&tft, (ili9340_color_t){ 0, 0x3f, 0 }, 25600);
  ili9340_fill(&tft, (ili9340_color_t){ 0, 0, 0x3f }, 25600);

  while (1);

  return 0;
}
