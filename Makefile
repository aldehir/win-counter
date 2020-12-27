mmcu = atmega328p
f_cpu = 16000000

sources := $(wildcard *.c)
objects := $(patsubst %.c,%.o,$(sources))

# Glyphs to generate
font = assets/fonts/Montserrat-Regular.ttf
font_size = 100
glyphs = 0 1 2 3 4 5 6 7 8 9
glyph_images = $(patsubst %,.generated/glyph-%.png,$(glyphs))
separator_image = assets/separator.png

CC = avr-gcc
CFLAGS = -mmcu=$(mmcu) -DF_CPU=$(f_cpu) -Wall -Wextra -c -g -std=gnu11 \
				 -Os -ffunction-sections -fdata-sections -flto -fno-fat-lto-objects

AVRDUDE = avrdude

.PHONY: all flash watch clean dirs

all: dirs win-counter

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.s: %.c
	$(CC) $(CFLAGS) -Wa,-a,-ad $< > $@

dirs:
	@[ ! -d .generated ] && mkdir .generated || :

.generated/glyph-%.png: scripts/generate_glyph_image.py
	scripts/generate_glyph_image.py -o $@ $(font) $(font_size) $(@:.generated/glyph-%.png=%)

resources.c: $(glyph_images) $(separator_image) scripts/compile_asset.py scripts/compile_assets.sh
	scripts/compile_assets.sh $(@:%.c=%.h) $@ $(glyph_images) $(separator_image)

win-counter: $(objects)
	$(CC) -mmcu=$(mmcu) $^ -o $@

clean:
	rm -f win-counter *.o

flash: win-counter
	avrdude -v -pm328p -carduino -P$(PORT) -b57600 -D -Uflash:w:win-counter:e

watch:
	-socat -d $(PORT),b115200,raw,echo=0 -
