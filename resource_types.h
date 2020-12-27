#pragma once

#include <stdint.h>

typedef struct {
  uint16_t width;
  uint16_t height;
  uint16_t lines;
  uint8_t *data;
} image_t;
