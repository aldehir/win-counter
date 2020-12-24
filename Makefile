mmcu = atmega328p
f_cpu = 16000000

sources := $(wildcard *.c)
objects := $(patsubst %.c,%.o,$(sources))

CC = avr-gcc
CFLAGS = -mmcu=$(mmcu) -DF_CPU=$(f_cpu) -Wall -Wextra -c -g -std=gnu11 \
				 -Os -ffunction-sections -fdata-sections -flto -fno-fat-lto-objects

AVRDUDE = avrdude

.PHONY: all flash clean

all: win-counter

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.s: %.c
	$(CC) $(CFLAGS) -Wa,-a,-ad $< > $@

win-counter: $(objects)
	$(CC) -mmcu=$(mmcu) $^ -o $@

clean:
	rm -f win-counter *.o

flash: win-counter
	avrdude -v -pm328p -carduino -P$(PORT) -b57600 -D -Uflash:w:win-counter:e
