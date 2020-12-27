#pragma once

#include "spi.h"


typedef struct {
  SPI_DEVICE_MEMBERS

  pin_config_t rst_pin;
  pin_config_t dcx_pin;
} ili9340_t;


typedef union {
  struct {
    uint8_t manufacturer_id;
    uint8_t module_version_id;
    uint8_t module_id;
  };

  uint8_t data[3];
} ili9340_display_info_t;


typedef struct {
  uint8_t data[4];
} ili9340_display_status_t;


typedef struct {
  uint8_t data;
} ili9340_madctl_t;


typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} ili9340_color_t;


void ili9340_init(ili9340_t *dev);
void ili9340_reset(ili9340_t *dev);

void ili9340_sleep_out(ili9340_t *dev);
void ili9340_display_on(ili9340_t *dev);

void ili9340_get_display_info(ili9340_t *dev, ili9340_display_info_t *info);
void ili9340_get_display_status(ili9340_t *dev, ili9340_display_status_t *status);
void ili9340_get_madctl(ili9340_t *dev, ili9340_madctl_t *info);

void ili9340_set_draw_region(ili9340_t *dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ili9340_memory_write(ili9340_t *dev);

void ili9340_push(ili9340_t *dev, const ili9340_color_t color);
void ili9340_fill(ili9340_t *dev, const ili9340_color_t color, const uint32_t len);
