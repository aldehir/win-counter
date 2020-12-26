#!/usr/bin/env python3

import os
import sys
import argparse

from collections import namedtuple
from itertools import zip_longest
from PIL import Image


Line = namedtuple("Line", ["x", "y", "points"])


class LinePrinter(object):
    def __init__(self, enable=1, file=sys.stderr):
        self.enable = enable
        self.file = file
        self.x = 0
        self.y = 0

    def print(self, line):
        if not self.enable:
            return

        if line.y != self.y:
            self.file.write("\n")
            self.file.flush()

        self.y = line.y

        self.move_to_col(line.x)
        for p in line.points:
            self.print_point(p)

    def move_to_col(self, col):
        col = col + 1
        self.file.write(f"\x1b[{col}G")

    def print_point(self, p):
        self.file.write(f"\x1b[48;2;{p};{p};{p}m \x1b[0m")

    def end(self):
        if not self.enable:
            return
        self.file.write("\n")


def iteralpha(img):
    for idx, value in enumerate(img.getdata(3)):
        y, x = divmod(idx, img.width)
        yield (x, y), value


def append_uint16(l, v):
    # Little-endian order to simplify code
    l.append(v & 0xFF)
    l.append((v << 8) & 0xFF)


def append_uint8(l, v):
    l.append(v & 0xFF)


def append_line(l, line):
    append_uint16(l, line.x)
    append_uint16(l, line.y)

    for point in line.points:
        append_uint8(l, point)

    append_uint8(l, 0)


parser = argparse.ArgumentParser()
parser.add_argument(
    "-p", "--print", action="store_true", default=False, help="print lines to stderr"
)
parser.add_argument("-c", "--columns", default=80, help="columns")
parser.add_argument(
    "-a", "--append", action="store_true", default=False, help="append to files"
)
parser.add_argument("input_file", help="input file")
parser.add_argument("var", help="variable name")
parser.add_argument("output_header", help="output_header")
parser.add_argument("output_source", help="output_source")
args = parser.parse_args()

open_mode = "w"
if args.append:
    open_mode = "a"

output_header = open(args.output_header, open_mode)
output_source = open(args.output_source, open_mode)

image = Image.open(args.input_file)
printer = LinePrinter(args.print)

if image.mode != "RGBA":
    print("ERROR: Must provide an image with transparency", file=sys.stderr)

sparse = []

append_uint16(sparse, image.width)
append_uint16(sparse, image.height)

current_line = None

for (x, y), value in iteralpha(image):
    if current_line is not None:
        if value == 0 or y != current_line.y:
            printer.print(current_line)
            append_line(sparse, current_line)
            current_line = None

    if value > 0:
        if current_line is None:
            current_line = Line(x, y, [value])
        else:
            current_line.points.append(value)

if current_line is not None:
    printer.print(current_line)
    append_line(sparse, current_line)

printer.end()

bytes_per_line = ((args.columns - 4 - 5) // 6) + 1

input_basename = os.path.basename(args.input_file)

if not args.append:
    output_header.write("#pragma once\n\n")
    output_header.write("#include <stdint.h>\n")
    output_header.write("#include <avr/pgmspace.h>\n\n")

    relative_header = os.path.relpath(args.output_header, os.path.dirname(args.output_source))
    output_source.write(f'#include "{relative_header}"\n\n')

output_header.write(f"/* {input_basename} */\n")
output_header.write(f"#define {args.var}_width {image.width}\n")
output_header.write(f"#define {args.var}_height {image.height}\n")
output_header.write(f"extern const uint8_t {args.var}[] PROGMEM;\n\n")

output_source.write(f"/* {input_basename} */\n")
output_source.write("const uint8_t %s[] PROGMEM = {\n" % args.var)

grouper = zip_longest(*[iter(sparse)] * bytes_per_line, fillvalue=None)
for group in grouper:
    line = ", ".join(f"0x{b:02x}" for b in group if b is not None)
    output_source.write(f"    {line},\n")

output_source.write("};\n\n")

output_source.close()
output_header.close()

print(f"Total size: {len(sparse)} bytes", file=sys.stderr)
