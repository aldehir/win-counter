# Sparse Image Format

The generated `resources.h` file contains the following definition of an image
type:

    typedef struct {
      uint16_t width;
      uint16_t height;
      uint16_t lines;
      uint8_t *data;
    } image_t;

The `data` member contains a sequence of lines that define the image. The
`lines` member defines how many lines are present in `data`.

A line is represented by its `(x, y)` coordinates as two unsigned 16-bit
integers in little-endian, followed by `n` bytes representing the alpha values
for `(x, y), (x+1, y), ..., (x+n, y)`. The line is terminated with a NULL byte
  `0x00`.

    ┌──────╥───┬───┬───┬───┬───┬───┬───┬───┐
    │      ║ 7 │ 6 │ 5 │ 4 │ 3 │ 2 │ 1 │ 0 │
    ╞══════╬═══╧═══╧═══╧═══╧═══╧═══╧═══╧═══╡
    │    0 ║             x low             │
    ├──────╫───────────────────────────────┤
    │    8 ║            x high             │
    ├──────╫───────────────────────────────┤
    │   16 ║             y low             │
    ├──────╫───────────────────────────────┤
    │   24 ║            y high             │
    ├──────╫───────────────────────────────┤
    │   32 ║      alpha[0]...alpha[n]      │
    ├──────╫───┬───┬───┬───┬───┬───┬───┬───┤
    │ 32+n ║ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │
    ╘══════╩═══╧═══╧═══╧═══╧═══╧═══╧═══╧═══╛

