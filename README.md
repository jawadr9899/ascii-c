# ascii-c

A lightweight command-line tool written in C that converts images into ASCII art rendered directly in your terminal.

```
@@@@@@@@##########%%%%%***+++===---::.. .....::---===+++***%%%%%##########@@@@@@@@
```

---

## Features

- Convert any image (PNG, JPG, etc.) into ASCII art
-  Brightness-based character mapping using the luminance formula
-  10-level ASCII palette from dark to light: ` .:-=+*#%@`
-  Renders output as a 30×30 ASCII grid in the terminal
-  Optional `-i` flag to inspect image metadata (dimensions, color type, channels, bit depth)
   No external dependencies — `stb_image` is bundled in `headers/`

---

## Dependencies

- [`stb_image`](https://github.com/nothings/stb/blob/master/stb_image.h) — single-header image loading library (already included in `headers/stb_image.h`)
- Standard C libraries: `stdio.h`, `stdlib.h`, `string.h`

No additional installs required.

---

## Building

Compile with `gcc` (or any C compiler):

```bash
gcc main.c -o ascii-c -lm
```

Or with `cc`:

```bash
cc main.c -o ascii-c -lm
```

---

## Usage

### Convert an image to ASCII art

```bash
./ascii-c <image_path>
```

**Example:**

```bash
./ascii-c images/sample.png
```

### Print image metadata (info mode)

```bash
./ascii-c <image_path> -i
```

**Example:**

```bash
./ascii-c images/sample.png -i
```

This will print details like:

```
PNG {
 name: images/sample.png
 width: 512
 height: 512
 pixels: 262144
 chunk-length: 13
 chunk-type: IHDR
 bit-depth: 8
 color-data:{
  color-type: Truecolor
  channels: 3
  }
}
```

---

## How It Works

1. **Load image** — `stb_image` decodes the image into raw RGB pixel data.
2. **Scale down** ��� The image is sampled down to a 30×30 grid.
3. **Brightness mapping** — Each pixel's brightness is computed using the standard luminance formula:
   ```
   brightness = 0.299 * R + 0.587 * G + 0.114 * B
   ```
4. **ASCII mapping** — The brightness value (0–255) is mapped to one of 10 characters in the palette:
   ```
    .:-=+*#%@
   ```
   (space = darkest, `@` = brightest)
5. **Render** — Each character is printed twice side-by-side to compensate for the typical 2:1 height-to-width ratio of terminal fonts.

---

## Project Structure

```
ascii-c/
├── main.c              # Entry point — image loading, ASCII rendering, CLI handling
├── headers/
│   ├── png.h           # Custom PNG struct definition
│   └── stb_image.h     # stb_image single-header library
└── images/             # Place your input images here
```

---

## License

No license has been set for this project yet. All rights reserved by the author.
