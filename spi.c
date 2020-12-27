#include "spi.h"

#include <avr/cpufunc.h>
#include <util/delay.h>

spi_t spi = {&SPCR, &SPSR, &SPDR, B3, B4, B5, B2};
spi_device_t *selected = 0;


void spi_init(spi_t *spi) {
  set_pin_output(spi->mosi);
  set_pin_input(spi->miso);
  set_pin_output(spi->sck);

  set_pin_output(spi->cs);
  write_pin_high(spi->cs);

  set_bit(*spi->spcr, SPE);
  set_bit(*spi->spcr, MSTR);

  clear_bit(*spi->spcr, SPR1);
  clear_bit(*spi->spcr, SPR0);
  set_bit(*spi->spsr, SPI2X);
}


uint8_t spi_send_byte(spi_t *spi, uint8_t b) {
  *spi->spdr = b;
  loop_until_bit_is_set(*spi->spsr, SPIF);
  return *spi->spdr;
}


uint8_t spi_recv_byte(spi_t *spi) {
  return spi_send_byte(spi, 0x00);
}

void spi_send(spi_t *spi, const uint8_t *data, const size_t size) {
  const uint8_t *end = data + size;
  while (data != end) {
    spi_send_byte(spi, *data);
    data++;
  }
}

void spi_recv(spi_t *spi, uint8_t *data, const size_t size) {
  const uint8_t *end = data + size;
  while (data != end) {
    *data = spi_recv_byte(spi);
    data++;
  }
}

void spi_device_init(spi_device_t *dev) {
  set_pin_output(dev->cs);
  write_pin_high(dev->cs);
}

void _spi_device_select(spi_device_t *dev) {
  if (selected == dev) {
    return;
  } else if (selected != NULL) {
    _spi_device_deselect(selected);
  }

  write_pin_low(dev->cs);
  // _delay_ms(100);
  selected = dev;
}

void _spi_device_deselect(spi_device_t *dev) {
  write_pin_high(dev->cs);
  // _delay_ms(100);
}
