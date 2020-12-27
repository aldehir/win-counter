#include "renderer.h"
#include "ili9340.h"

#include <avr/pgmspace.h>


static void fill_to(render_context_t *ctx,
    ili9340_color_t *color,
    uint16_t *x, uint16_t *y,
    uint16_t dest_x, uint16_t dest_y,
    uint16_t width) {

  while (*x < dest_x || *y < dest_y) {
    ili9340_push(ctx->device, *color);
    *x += 1;

    if (*x >= width) {
      *x = 0;
      *y += 1;
    }
  }
}


#define to_6bit_color(c) ((c * 63UL) / 255)

static void rgb_to_ili9340_color(const rgb_t *in, ili9340_color_t *out) {
  out->red = to_6bit_color(in->red);
  out->green = to_6bit_color(in->green);
  out->blue = to_6bit_color(in->blue);
}


#define color_alpha_blend(alpha, fg, bg) (((fg * (uint16_t)alpha) + (bg * (255UL - alpha))) / 255)

static void rgb_alpha_blend(uint8_t alpha, const rgb_t *fg, const rgb_t *bg, rgb_t *out) {
  out->red = color_alpha_blend(alpha, fg->red, bg->red);
  out->green = color_alpha_blend(alpha, fg->green, bg->green);
  out->blue = color_alpha_blend(alpha, fg->blue, bg->blue);
}


void render_rectangle(render_context_t *ctx,
    uint16_t x, uint16_t y, uint16_t width, uint16_t height,
    const rgb_t *color) {

  ili9340_color_t color_6bit;
  rgb_to_ili9340_color(color, &color_6bit);

  ili9340_set_draw_region(ctx->device, x, y, width, height);
  ili9340_memory_write(ctx->device);

  for (uint16_t i = 0; i < width; i++) {
    for (uint16_t j = 0; j < height; j++) {
      ili9340_push(ctx->device, color_6bit);
    }
  }
}


void render_glyph(render_context_t *ctx, uint16_t x, uint16_t y,
    const image_t *glyph, const rgb_t *fg, const rgb_t *bg) {

  ili9340_color_t fg_6bit, bg_6bit;

  rgb_to_ili9340_color(fg, &fg_6bit);
  rgb_to_ili9340_color(bg, &bg_6bit);

  uint8_t *ptr = glyph->data;
  uint16_t lines = glyph->lines;

  uint16_t current_x = 0;
  uint16_t current_y = 0;
  uint16_t line_x = 0;
  uint16_t line_y = 0;

  ili9340_set_draw_region(ctx->device, x, y, glyph->width, glyph->height);
  ili9340_memory_write(ctx->device);

  rgb_t blend;
  ili9340_color_t blend_6bit;

  for (uint16_t line = 0; line < lines; line++) {
    line_x = pgm_read_word(ptr);
    line_y = pgm_read_word(ptr + 2);
    ptr += 4;

    fill_to(ctx, &bg_6bit, &current_x, &current_y, line_x, line_y, glyph->width);

    while (1) {
      uint8_t alpha = pgm_read_byte(ptr);
      ptr++;

      if (!alpha) {
        break;
      }

      rgb_alpha_blend(alpha, fg, bg, &blend);
      rgb_to_ili9340_color(&blend, &blend_6bit);
      ili9340_push(ctx->device, blend_6bit);
      current_x++;
    }
  }

  fill_to(ctx, &bg_6bit, &current_x, &current_y, 0, glyph->height, glyph->width);
}
