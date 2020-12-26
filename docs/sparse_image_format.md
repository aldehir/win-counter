# Sparse Image Format

Below is the sparse format used to represent the images in this project. The
images 8-bit alpha channels, which allows us to programmatically assign the
coloring. Images are represented as a sequence of contiguous horizontal lines.

    ┌──────╥───┬───┬───┬───┬───┬───┬───┬───┐
    │      ║ 7 │ 6 │ 5 │ 4 │ 3 │ 2 │ 1 │ 0 │
    ╞══════╬═══╧═══╧═══╧═══╧═══╧═══╧═══╧═══╡
    │ 0..3 ║        header [0..32]         │
    ├──────╫───────────────────────────────┤
    │ 4..n ║         lines [0..n]          │
    ╘══════╩═══════════════════════════════╛


## Header

The header consists of the image width and height as two unsigned 16-bit
integers.

    ┌────╥───┬───┬───┬───┬───┬───┬───┬───┐
    │    ║ 7 │ 6 │ 5 │ 4 │ 3 │ 2 │ 1 │ 0 │
    ╞════╬═══╧═══╧═══╧═══╧═══╧═══╧═══╧═══╡
    │  0 ║          width high           │
    ├────╫───────────────────────────────┤
    │  8 ║           width low           │
    ├────╫───────────────────────────────┤
    │ 16 ║          height high          │
    ├────╫───────────────────────────────┤
    │ 24 ║          height low           │
    ╘════╩═══════════════════════════════╛


## Line

A line is represented by its `(x, y)` coordinates as two unsigned 16-bit
integers, followed by `n` bytes representing the alpha values for
`(x, y), (x+1, y), ..., (x+n, y)`. The line is terminated with a NULL byte
`0x00`.

    ┌──────╥───┬───┬───┬───┬───┬───┬───┬───┐
    │      ║ 7 │ 6 │ 5 │ 4 │ 3 │ 2 │ 1 │ 0 │
    ╞══════╬═══╧═══╧═══╧═══╧═══╧═══╧═══╧═══╡
    │    0 ║            x high             │
    ├──────╫───────────────────────────────┤
    │    8 ║             x low             │
    ├──────╫───────────────────────────────┤
    │   16 ║            y high             │
    ├──────╫───────────────────────────────┤
    │   24 ║             y low             │
    ├──────╫───────────────────────────────┤
    │   32 ║      alpha[0]...alpha[n]      │
    ├──────╫───┬───┬───┬───┬───┬───┬───┬───┤
    │ 32+n ║ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │
    ╘══════╩═══╧═══╧═══╧═══╧═══╧═══╧═══╧═══╛

