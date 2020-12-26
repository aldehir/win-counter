#include "ili9340.h"

#include <avr/cpufunc.h>
#include <util/delay.h>

#include "debug.h"
#include "spi.h"


#define CMD_READ_DISPLAY_INFO 0x04
#define CMD_READ_DISPLAY_STATUS 0x09
#define CMD_READ_DISPLAY_MADCTL 0x0b

#define CMD_SLEEP_OUT 0x11
#define CMD_DISPLAY_ON 0x29
#define CMD_MEMORY_WRITE 0x2C

#define CMD_COLUMN_ADDRESS_SET 0x2A
#define CMD_PAGE_ADDRESS_SET 0x2B

#define CMD_GAMMA_SET 0x26
#define CMD_POSITIVE_GAMMA_CORRECTION 0xE0
#define CMD_NEGATIVE_GAMMA_CORRECTION 0xE1

#define CMD_POWER_CONTROL_1 0xC0
#define CMD_POWER_CONTROL_2 0xC1
#define CMD_VCOM_CONTROL_1 0xC5
#define CMD_VCOM_CONTROL_2 0xC7

#define CMD_FRAME_CONTROL_1 0xB1
#define CMD_DISPLAY_FUNCTION_CONTROL 0xB6

#define CMD_MEMORY_ACCESS_CONTROL 0x36
#define CMD_PIXEL_FORMAT 0x3A

#define CMD_NOOP 0x00

const uint8_t data_power_control_1[] = { 0x23 };
const uint8_t data_power_control_2[] = { 0x10 };
const uint8_t data_vcom_control_1[] = { 0x3e, 0x28 };
const uint8_t data_vcom_control_2[] = { 0x86 };
const uint8_t data_memory_access_control[] = { 0x28 };
const uint8_t data_pixel_format[] = { 0x66 };
const uint8_t data_frame_control_1[] = { 0x00, 0x18 };
const uint8_t data_display_function_control[] = { 0x08, 0x82, 0x27 };

const uint8_t data_gamma_set[] = { 0x01 };
const uint8_t data_positive_gamma_correction[] = {
  0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e,
  0xf1, 0x37, 0x07, 0x10, 0x03, 0x0e, 0x09,
  0x00
};

const uint8_t data_negative_gamma_correction[] = {
  0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31,
  0xc1, 0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36,
  0x0f
};


static void send_command(ili9340_t *dev, uint8_t cmd);
static void init_magic(ili9340_t *dev);


void ili9340_init(ili9340_t *dev) {
  spi_device_init((spi_device_t*)dev);

  set_pin_output(dev->rst_pin);
  set_pin_output(dev->dcx_pin);

  write_pin_low(dev->rst_pin);
  write_pin_high(dev->dcx_pin);

  ili9340_reset(dev);
  ili9340_sleep_out(dev);

  init_magic(dev);

  ili9340_display_on(dev);
}

static void send_command(ili9340_t *dev, uint8_t cmd) {
  write_pin_low(dev->dcx_pin);
  spi_device_send_byte(dev, cmd);
  write_pin_high(dev->dcx_pin);
}

static void init_magic(ili9340_t *dev) {
  send_command(dev, CMD_POWER_CONTROL_1);
  spi_device_send(dev, data_power_control_1, sizeof(data_power_control_1));

  send_command(dev, CMD_POWER_CONTROL_2);
  spi_device_send(dev, data_power_control_2, sizeof(data_power_control_2));

  send_command(dev, CMD_VCOM_CONTROL_1);
  spi_device_send(dev, data_vcom_control_1, sizeof(data_vcom_control_1));

  send_command(dev, CMD_VCOM_CONTROL_2);
  spi_device_send(dev, data_vcom_control_2, sizeof(data_vcom_control_2));

  send_command(dev, CMD_MEMORY_ACCESS_CONTROL);
  spi_device_send(dev, data_memory_access_control, sizeof(data_memory_access_control));

  send_command(dev, CMD_PIXEL_FORMAT);
  spi_device_send(dev, data_pixel_format, sizeof(data_pixel_format));

  send_command(dev, CMD_FRAME_CONTROL_1);
  spi_device_send(dev, data_frame_control_1, sizeof(data_frame_control_1));

  send_command(dev, CMD_DISPLAY_FUNCTION_CONTROL);
  spi_device_send(dev, data_display_function_control, sizeof(data_display_function_control));

  send_command(dev, CMD_GAMMA_SET);
  spi_device_send(dev, data_gamma_set, sizeof(data_gamma_set));

  send_command(dev, CMD_POSITIVE_GAMMA_CORRECTION);
  spi_device_send(dev, data_positive_gamma_correction, sizeof(data_positive_gamma_correction));

  send_command(dev, CMD_NEGATIVE_GAMMA_CORRECTION);
  spi_device_send(dev, data_negative_gamma_correction, sizeof(data_negative_gamma_correction));

  ili9340_set_draw_region(dev, 0, 0, 320, 240);
}


void ili9340_reset(ili9340_t *dev) {
  write_pin_high(dev->rst_pin);
  _delay_ms(5);
  write_pin_low(dev->rst_pin);
  _delay_ms(1);
  write_pin_high(dev->rst_pin);
  _delay_ms(150);
}


void ili9340_sleep_out(ili9340_t *dev) {
  spi_device_select(dev);
  send_command(dev, CMD_SLEEP_OUT);
}


void ili9340_display_on(ili9340_t *dev) {
  spi_device_select(dev);
  send_command(dev, CMD_DISPLAY_ON);
}


void ili9340_get_display_info(ili9340_t *dev, ili9340_display_info_t *info) {
  spi_device_select(dev);

  send_command(dev, CMD_READ_DISPLAY_INFO);
  spi_device_recv_byte(dev);
  spi_device_recv(dev, info->data, 3);
}


void ili9340_get_display_status(ili9340_t *dev, ili9340_display_status_t *status) {
  spi_device_select(dev);

  send_command(dev, CMD_READ_DISPLAY_STATUS);
  spi_device_recv_byte(dev);
  spi_device_recv(dev, status->data, 4);
}


void ili9340_get_madctl(ili9340_t *dev, ili9340_madctl_t *info) {
  spi_device_select(dev);

  send_command(dev, CMD_READ_DISPLAY_MADCTL);
  spi_device_recv_byte(dev);
  info->data = spi_device_recv_byte(dev);
}


void ili9340_set_draw_region(ili9340_t *dev, uint16_t x1, uint16_t y1, uint16_t width, uint16_t height) {
  uint16_t x2 = (x1 + width - 1);
  uint16_t y2 = (y1 + height - 1);

  uint8_t data[] = {
    x1 >> 8, x1 & 0xff, x2 >> 8, x2 & 0xff,
    y1 >> 8, y1 & 0xff, y2 >> 8, y2 & 0xff
  };

  spi_device_select(dev);

  send_command(dev, CMD_COLUMN_ADDRESS_SET);
  spi_device_send(dev, data, 4);

  send_command(dev, CMD_PAGE_ADDRESS_SET);
  spi_device_send(dev, data + 4, 4);
}


void ili9340_memory_write(ili9340_t *dev) {
  spi_device_select(dev);
  send_command(dev, CMD_MEMORY_WRITE);
}


void ili9340_fill(ili9340_t *dev, const ili9340_color_t color, const uint32_t len) {
  spi_device_select(dev);

  for (uint32_t i = 0; i < len; i++) {
    spi_device_send_byte(dev, color.red << 2);
    spi_device_send_byte(dev, color.green << 2);
    spi_device_send_byte(dev, color.blue << 2);
  }
}
