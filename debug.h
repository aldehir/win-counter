#pragma once

#include "serial.h"

#if !defined(DEBUG_SERIAL_INTERFACE)
#define DEBUG_SERIAL_INTERFACE serial0
#endif

#define debug(fmt, ...) serial_printf(&DEBUG_SERIAL_INTERFACE, fmt "\r\n", ##__VA_ARGS__)
