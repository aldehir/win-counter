#include <stdbool.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "timer.h"
#include "serial.h"
#include "debug.h"
#include "ili9340.h"
#include "renderer.h"
#include "counter.h"
#include "button.h"


ili9340_t tft = {&spi, B2, D6, D5};
render_context_t renderer;
counter_display_t display;
counter_t counter;

button_t win_button;
button_t loss_button;
button_t reset_button;

button_t *buttons[] = { &win_button, &loss_button, &reset_button, 0 };


static void on_win_event();
static void on_loss_event();
static void on_reset_event();

static void poll_buttons_once();


static void initialize(void) {
  timer_init();
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

  button_init(&win_button, D2, on_win_event);
  button_init(&loss_button, D3, on_loss_event);
  button_init(&reset_button, D4, on_reset_event);

  while (1) {
    poll_buttons_once();
  }

  return 0;
}


static void poll_buttons_once() {
  button_t **current = buttons;
  while (*current) {
    button_poll(*current);
    current++;
  }
}


static void on_win_event(const event_t *event) {
  if (event->pressed) {
    debug("Win button pressed");
    counter_add_win(&counter);
  } else {
    debug("Win button released");
  }
}


static void on_loss_event(const event_t *event) {
  if (event->pressed) {
    debug("Loss button pressed");
    counter_add_loss(&counter);
  } else {
    debug("Loss button released");
  }
}

static void on_reset_event(const event_t *event) {
  if(event->pressed) {
    debug("Reset button pressed");
    counter_reset(&counter);
  } else {
    debug("Reset button released");
  }
}
