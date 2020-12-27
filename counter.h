#pragma once

#include <stdint.h>

#include "renderer.h"

typedef struct {
  render_context_t *renderer;

  rect_t last_win_region;
  rect_t last_loss_region;
} counter_display_t;


typedef struct {
  uint8_t wins;
  uint8_t losses;

  counter_display_t *display;
} counter_t;


void counter_init(counter_t *counter, counter_display_t *display);
void counter_begin(counter_t *counter);
void counter_reset(counter_t *counter);
void counter_add_win(counter_t *counter);
void counter_add_loss(counter_t *counter);

void counter_display_init(counter_display_t *display, render_context_t *renderer);
void counter_display_begin(counter_display_t *display);
void counter_display_render_wins(counter_display_t *display, uint8_t wins);
void counter_display_render_losses(counter_display_t *display, uint8_t losses);
