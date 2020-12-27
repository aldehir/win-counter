#include <stdbool.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "serial.h"
#include "debug.h"
#include "ili9340.h"
#include "renderer.h"
#include "counter.h"


ili9340_t tft = {&spi, B2, D6, D5};
render_context_t renderer;
counter_display_t display;
counter_t counter;


static void initialize(void) {
  serial_init(&serial0, 115200);
  spi_init(&spi);

  ili9340_init(&tft);
  render_init(&renderer, &tft);
  counter_display_init(&display, &renderer);
  counter_init(&counter, &display);
}


int main(void) {
  initialize();

  counter_display_begin(&display);
  counter_begin(&counter);

  for (int i = 0; i < 999; i++) {
    counter_add_win(&counter);
    counter_add_loss(&counter);
  }

  while (1);

  return 0;
}
