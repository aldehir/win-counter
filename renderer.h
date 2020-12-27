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


void render_rectangle(render_context_t *ctx,
    uint16_t x, uint16_t y, uint16_t width, uint16_t height,
    const rgb_t *color);

#define render_background(ctx, color) \
  render_rectangle((ctx), 0, 0, ILI9340_TFT_WIDTH, ILI9340_TFT_HEIGHT, (color))

void render_glyph(render_context_t *ctx, uint16_t x, uint16_t y,
    const image_t *glyph, const rgb_t *fg, const rgb_t *bg);
