#pragma once

#include <stddef.h>
#include <stdint.h>

#include "common.h"


typedef struct {
  volatile uint8_t *spcr;
  volatile uint8_t *spsr;
  volatile uint8_t *spdr;

  pin_config_t mosi;
  pin_config_t miso;
  pin_config_t sck;
  pin_config_t cs;
} spi_t;


#define SPI_DEVICE_MEMBERS \
  spi_t *spi;              \
  pin_config_t cs;

typedef struct {
  SPI_DEVICE_MEMBERS
} spi_device_t;


extern spi_t spi;


void spi_init(spi_t *spi);
uint8_t spi_send_byte(spi_t *spi, uint8_t b);
uint8_t spi_recv_byte(spi_t *spi);
void spi_send(spi_t *spi, const uint8_t *data, const size_t size);
void spi_recv(spi_t *spi, uint8_t *data, const size_t size);

void spi_device_init(spi_device_t *dev);
void _spi_device_select(spi_device_t *dev);
void _spi_device_deselect(spi_device_t *dev);

#define spi_device_select(dev) _spi_device_select((spi_device_t*)dev)
#define spi_device_deselect(dev) _spi_device_deselect((spi_device_t*)dev)

#define spi_device_send_byte(dev, b) spi_send_byte(((spi_device_t*)dev)->spi, (b))
#define spi_device_send(dev, data, size) spi_send(((spi_device_t*)dev)->spi, (data), (size))
#define spi_device_recv_byte(dev) spi_recv_byte(((spi_device_t*)dev)->spi)
#define spi_device_recv(dev, data, size) spi_recv(((spi_device_t*)dev)->spi, (data), (size))
