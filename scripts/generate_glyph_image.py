#!/usr/bin/env python3

import argparse

from PIL import Image, ImageDraw, ImageFont


parser = argparse.ArgumentParser()
parser.add_argument("-o", "--output", required=True, help="output file")
parser.add_argument("font", help="ttf font")
parser.add_argument("size", type=int, help="font size in points")
parser.add_argument("glyph", help="glyph to generate")

args = parser.parse_args()

font_file = args.font
font_size = args.size
glyph = args.glyph[0]

font = ImageFont.truetype(font_file, font_size)

print(f"Processing glyph {glyph}")

font_width, _ = font.getsize(glyph)
image_size = (font_width, font_size)

image = Image.new("RGBA", image_size, (0, 0, 0, 0))

offset = (0, font_size * 7 / 8)

draw = ImageDraw.Draw(image)
draw.text(offset, glyph, font=font, fill=(255, 255, 255, 255), anchor="ls")

image_path = args.output
image.save(image_path, "PNG")
print(f" - written to {image_path}")
