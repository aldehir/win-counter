#include "common.h"
#include "counter.h"
#include "resources.h"


#define ANCHOR_TOP 0x01
#define ANCHOR_BOTTOM 0x02
#define ANCHOR_LEFT 0x04
#define ANCHOR_RIGHT 0x08

#define RENDER_MAX_DIGITS 2

#define center_x(img) (((ILI9340_TFT_WIDTH - (img).width) / 2) - 1)
#define center_y(img) (((ILI9340_TFT_HEIGHT - (img).height) / 2) - 1)


rgb_t background_color = { 0x23, 0x25, 0x2b };

rgb_t white_color = { 0xff, 0xff, 0xff };
rgb_t red_color = { 0xff, 0x00, 0x66 };
rgb_t green_color = { 0x00, 0xff, 0xa8 };
rgb_t gold_color = { 0xff, 0xea, 0x00 };


const image_t *digits[] = {
  &glyph_0, &glyph_1, &glyph_2, &glyph_3, &glyph_4,
  &glyph_5, &glyph_6, &glyph_7, &glyph_8, &glyph_9,
};


void counter_init(counter_t *counter, counter_display_t *display) {
  counter->wins = counter->losses = 0;
  counter->display = display;
}


void counter_begin(counter_t *counter) {
  counter_reset(counter);
}


void counter_reset(counter_t *counter) {
  counter->wins = counter->losses = 0;

  if (counter->display) {
    counter_display_render_wins(counter->display, 0);
    counter_display_render_losses(counter->display, 0);
  }
}


void counter_add_win(counter_t *counter) {
  counter->wins++;

  if (counter->display) {
    counter_display_render_wins(counter->display, counter->wins);
  }
}


void counter_add_loss(counter_t *counter) {
  counter->losses++;

  if (counter->display) {
    counter_display_render_losses(counter->display, counter->losses);
  }
}


void counter_display_init(counter_display_t *display, render_context_t *renderer)  {
  display->renderer = renderer;
  display->last_win_region = (rect_t){ 0, 0, 0, 0 };
  display->last_loss_region = (rect_t){ 0, 0, 0, 0 };
}


void counter_display_begin(counter_display_t *display) {
  render_background(display->renderer, &background_color);

  render_image(display->renderer,
    center_x(separator), center_y(separator),
    &separator, &white_color, &background_color
  );
}


static void display_number(counter_display_t *display,
    uint8_t number, uint16_t x, uint16_t y, const rgb_t *color,
    uint8_t anchor, rect_t *last_region) {

  const image_t *glyphs[RENDER_MAX_DIGITS];

  const image_t **glyph_begin = glyphs - 1;
  const image_t **glyph_end = glyphs + RENDER_MAX_DIGITS;
  const image_t **glyph = glyph_end - 1;

  uint16_t total_width = 0;
  uint16_t max_height = 0;

  do {
    uint8_t digit = number % 10;
    number /= 10;

    *glyph = digits[digit];
    total_width += (*glyph)->width;
    max_height = MAX(max_height, (*glyph)->height);

    glyph--;
  } while(number && glyph != glyph_begin);

  uint16_t current_x = (anchor & ANCHOR_LEFT) ? x : x - total_width + 1;
  uint16_t current_y = (anchor & ANCHOR_TOP) ? y : y - max_height + 1;

  if (last_region) {
    render_rectangle(display->renderer, last_region, &background_color);
    *last_region = (rect_t){ current_x, current_y, total_width, max_height };
  }

  glyph++;

  while (glyph != glyph_end)  {
    render_image(display->renderer, current_x, current_y, *glyph, color, &background_color);
    current_x += (*glyph)->width;
    glyph++;
  }
}


void counter_display_render_wins(counter_display_t *display, uint8_t wins) {
  display_number(display,
      wins,
      ((ILI9340_TFT_WIDTH / 2) - 10), ((ILI9340_TFT_HEIGHT / 2) - 10),
      &green_color,
      ANCHOR_BOTTOM | ANCHOR_RIGHT,
      &display->last_win_region);
}


void counter_display_render_losses(counter_display_t *display, uint8_t losses) {
  display_number(display,
      losses,
      ((ILI9340_TFT_WIDTH / 2) + 5), ((ILI9340_TFT_HEIGHT / 2) + 5),
      &red_color,
      ANCHOR_TOP | ANCHOR_LEFT,
      &display->last_loss_region);
}
