#pragma once

#include <stdint.h>

#include "common.h"
#include "timer.h"


#define BUTTON_DEBOUNCE_TIME_MS 50


typedef struct {
  uint8_t pressed;
  timer_t event_time;
} event_t;

typedef void (*button_event_cb)(const event_t*);

typedef struct {
  pin_config_t pin;

  button_event_cb on_event;

  uint8_t state;
  timer_t last_state_change;
  uint8_t pending;
} button_t;


void button_init(button_t *button, pin_config_t pin, button_event_cb on_event);
void button_poll(button_t *button);
