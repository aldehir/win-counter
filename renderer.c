#include "renderer.h"
#include "ili9340.h"

#include <avr/pgmspace.h>


#define set_ctx(ctx, x, y, width, height) do {    \
    (ctx)->_x = (x);                              \
    (ctx)->_y = (y);                              \
  } while(0);

#define wrap_coordinates(ctx) do {          \
    if ((ctx)->_x == (ctx)->_width) {       \
      (ctx)->_x = 0;                        \
      (ctx)->_y++;                          \
    }                                       \
  } while(0)


static void set_region(render_context_t *ctx,
    uint16_t x, uint16_t y,
    uint16_t width, uint16_t height) {

  ili9340_set_draw_region(ctx->device, x, y, width, height);
  ili9340_memory_write(ctx->device);

  ctx->_x = ctx->_y = 0;
  ctx->_width = width;
  ctx->_height = height;
}


static void fill_to(render_context_t *ctx, ili9340_color_t *color,
    uint16_t dest_x, uint16_t dest_y) {
  while (ctx->_x < dest_x || ctx->_y < dest_y) {
    ctx->_x++;
    wrap_coordinates(ctx);
    ili9340_push(ctx->device, *color);
  }
}


static void read_line_coordinates(uint8_t **ptr, uint16_t *x, uint16_t *y) {
    *x = pgm_read_word(*ptr);
    *y = pgm_read_word((*ptr) + 2);
    *ptr += 4;
}


static uint8_t read_alpha(uint8_t **ptr) {
    uint8_t alpha = pgm_read_byte(*ptr);
    (*ptr)++;
    return alpha;
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


void render_init(render_context_t *ctx, ili9340_t *device) {
  ctx->device = device;
  ctx->_x = ctx->_y = ctx->_width = ctx->_height = 0;
}


void render_rectangle(render_context_t *ctx, const rect_t *rect, const rgb_t *color) {
  if (rect->width == 0 || rect->height == 0) {
    return;
  }

  ili9340_color_t color_6bit;
  rgb_to_ili9340_color(color, &color_6bit);

  set_region(ctx, rect->x, rect->y, rect->width, rect->height);
  fill_to(ctx, &color_6bit, 0, rect->height);
}


void render_image(render_context_t *ctx, uint16_t x, uint16_t y,
    const image_t *image, const rgb_t *fg, const rgb_t *bg) {
  ili9340_color_t fg_6bit, bg_6bit;

  rgb_to_ili9340_color(fg, &fg_6bit);
  rgb_to_ili9340_color(bg, &bg_6bit);

  uint8_t *ptr = image->data;
  uint16_t lines = image->lines;
  uint16_t line_x;
  uint16_t line_y;
  uint8_t alpha;

  set_region(ctx, x, y, image->width, image->height);

  rgb_t blend;
  ili9340_color_t blend_6bit;

  for (uint16_t line = 0; line < lines; line++) {
    read_line_coordinates(&ptr, &line_x, &line_y);
    fill_to(ctx, &bg_6bit, line_x, line_y);

    while (1) {
      alpha = read_alpha(&ptr);
      if (!alpha) {
        break;
      }

      rgb_alpha_blend(alpha, fg, bg, &blend);
      rgb_to_ili9340_color(&blend, &blend_6bit);

      ili9340_push(ctx->device, blend_6bit);
      ctx->_x++;
    }

    wrap_coordinates(ctx);
  }

  fill_to(ctx, &bg_6bit, 0, image->height);
}
