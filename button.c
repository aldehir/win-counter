#include "button.h"


void button_init(button_t *button, pin_config_t pin, button_event_cb on_event) {
  button->pin = pin;
  button->state = 0;
  button->last_state_change = 0;
  button->on_event = 0;
  button->on_event = on_event;

  set_pin_input(button->pin);
}


static void on_button_state_change(button_t *button) {
  event_t event = { button->state, button->last_state_change };
  if (button->on_event) {
    (button->on_event)(&event);
  }
}


void button_poll(button_t *button) {
  uint8_t state = read_pin(button->pin);

  if (button->state != state) {
    button->state = state;
    button->last_state_change = timer_read();
    button->pending = 1;
  } else {
    if (button->pending) {
      if (timer_elapsed(button->last_state_change) >= BUTTON_DEBOUNCE_TIME_MS) {
        on_button_state_change(button);
        button->pending = 0;
      }
    }
  }
}
