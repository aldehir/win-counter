#pragma once

#include "ili9340.h"
#include "resource_types.h"


typedef struct {
  ili9340_t *device;
} render_context_t;

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} rgb_t;


#define color_8to6bit(c) ((c * 63UL) / 255)
#define rgb_to_ili9340_color(c) ((ili9340_color_t){                       \
      color_8to6bit(c.red), color_8to6bit(c.green), color_8to6bit(c.blue) \
    })

#define color_alpha_blend(bg, fg, alpha) \
    (((fg * (uint16_t)alpha) + (bg * (255UL - alpha))) / 255)

#define rgb_alpha_blend(bg, fg, alpha) ((rgb_t){         \
      color_alpha_blend(bg.red, fg.red, alpha),          \
      color_alpha_blend(bg.green, fg.green, alpha),      \
      color_alpha_blend(bg.blue, fg.blue, alpha)         \
    })


void draw_glyph(render_context_t *ctx, uint16_t x, uint16_t y,
    const image_t *glyph, rgb_t fg, rgb_t bg);

