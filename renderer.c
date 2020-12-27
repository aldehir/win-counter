#include "renderer.h"

#include <avr/pgmspace.h>


void draw_glyph(render_context_t *ctx, uint16_t x, uint16_t y,
    const image_t *glyph, rgb_t fg, rgb_t bg) {

  uint8_t *pos = glyph->data;
  uint16_t lines = glyph->lines;

  ili9340_set_draw_region(ctx->device, x, y, glyph->width, glyph->height);
  ili9340_memory_write(ctx->device);

  uint16_t current_x = 0;
  uint16_t current_y = 0;

  uint16_t line_x = 0;
  uint16_t line_y = 0;

  for (uint16_t line = 0; line < lines; line++) {
    line_x = pgm_read_word(pos);
    pos += 2;

    line_y = pgm_read_word(pos);
    pos += 2;

    // Draw until first y
    while (current_y != line_y) {
      ili9340_fill(ctx->device, rgb_to_ili9340_color(bg), glyph->width - current_x);
      current_x = 0;
      current_y++;
    }

    // Draw until first x
    while (current_x != line_x) {
      ili9340_push(ctx->device, rgb_to_ili9340_color(bg));
      current_x++;
    }

    while (1) {
      uint8_t alpha = pgm_read_byte(pos);
      pos++;

      if (!alpha) {
        break;
      }

      rgb_t blended_color = rgb_alpha_blend(bg, fg, alpha);
      ili9340_push(ctx->device, rgb_to_ili9340_color(blended_color));
      current_x++;
    }
  }

  // Finish
  while (current_y != glyph->height) {
    ili9340_fill(ctx->device, rgb_to_ili9340_color(bg), glyph->width - current_x);
    current_x = 0;
    current_y++;
  }
}
