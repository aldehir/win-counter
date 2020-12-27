#include <stdbool.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "serial.h"
#include "debug.h"
#include "ili9340.h"
#include "renderer.h"
#include "resources.h"


ili9340_t tft = {&spi, B2, D6, D5};
render_context_t renderer = {&tft};

rgb_t background_color = { 0x23, 0x25, 0x2b };
rgb_t foreground_color = { 0xff, 0xff, 0xff };

rgb_t red_color = { 0xff, 0x00, 0x66 };
rgb_t green_color = { 0x00, 0xff, 0xa8 };
rgb_t gold_color = { 0xff, 0xea, 0x00 };


static void initialize(void) {
  serial_init(&serial0, 115200);
  spi_init(&spi);
  ili9340_init(&tft);
}


int main(void) {
  initialize();

  render_background(&renderer, &background_color);

  uint16_t x = ((320 - glyph_1.width - 2 * glyph_0.width) / 2) - 1;
  uint16_t y = ((240 - glyph_1.height) / 2) - 1;

  render_glyph(&renderer, x, y, &glyph_1, &red_color, &background_color);
  render_glyph(&renderer, x + glyph_1.width, y, &glyph_0, &green_color, &background_color);
  render_glyph(&renderer, x + glyph_1.width + glyph_0.width, y, &glyph_0, &gold_color, &background_color);

  while (1);

  return 0;
}
