# GBR2PNG

**GBR2PNG** is a lightweight C utility to extract tile data from Gameboy Tile Designer (`.GBR`) files and convert them into a `.PNG`.

## Features

- Automatically identifies the number of tiles by scanning for tile data starting at offset `0xB4`
- Stops processing after finding a sequence of **3 empty tiles** to avoid exporting unnecessary tiles
- Written in standard C with no external library requirements
- Renders images using the original Gameboy DMG green-scale colors

## Conversion Process

| Component | Specification |
| ----------- | --------------- |
| Tile Size | `8x8` Pixels |
| Data Offset | `0xB4` (Standard .GBR header length) |
| Bytes per Tile | 64 bytes |
| Palette | 4-color DMG |
| Output Format | PPM (convertible to PNG) |

### DMG Color Palette

The tool uses the classic Gameboy DMG palette:

- **Color 0** (Lightest): RGB(155, 188, 15)
- **Color 1** (Light): RGB(139, 172, 15)
- **Color 2** (Dark): RGB(48, 98, 48)
- **Color 3** (Darkest): RGB(15, 56, 15)

## Usage

### Compilation

```bash
make
# or
gcc -o GBR2PNG src/main.c
```

### Running

```bash
./build/GBR2PNG <input_file.gbr>
```

The program will automatically:

1. Read the `.GBR` file
2. Extract tile data starting at offset `0xB4`
3. Process tiles until 3 consecutive empty tiles are found
4. Output the result as `output.ppm`

### Converting to PNG

If you need a **PNG** file, convert the **PPM** output using **ImageMagick**:

**macOS:**

```bash
brew install imagemagick
convert output.ppm output.png
```

**Linux (Debian/Ubuntu):**

```bash
sudo apt-get install imagemagick
convert output.ppm output.png
```

**Windows:**
Download ImageMagick from [imagemagick.org](https://imagemagick.org/script/download.php), then:

```bash
magick convert output.ppm output.png
```

Or use any image viewer that supports PPM format.

## Example of a `.GBR` file

### Header

Info:

```hex
47 42 4F 30 01 00 00 00    GBO0....
78 00 00 00 47 61 6D 65    x...Game
62 6F 79 20 54 69 6C 65    boy Tile
20 44 65 73 69 67 6E 65    Designe
72 00 00 00 00 00 00 00    r.......
00 00 32 2E 32 00 1D 01    ..2.2...
02 00 00 00 48 6F 6D 65    ....Home
3A 20 77 77 77 2E 63 61    : www.ca
73 65 6D 61 2E 6E 65 74    sema.net
2F 7E 68 70 6D 75 6C 64    /~hpmuld
65 72 00 00 00 00 00 00    er......
00 00 00 00 00 00 00 00    ........
00 00 00 00 00 00 00 00    ........
00 00 00 00 00 00 00 00    ........
00 00 00 00 00 00 00 00    ........
00 00 00 00 00 00 00 00    ........
00 00 00 00 00 00 00 00    ........
00 00 02 00 01 00 28 20    ......( 
00 00 00 00 00 00 00 00    ........
00 00 00 00 00 00 00 00    ........
00 00 00 00 00 00 00 00    ........
00 00 00 00 00 00 00 00    ........
08 00 08 00 80 00          ......
```

### Palettes

```hex
00 01 02 03  White  Light-Grey  Grey  Black
```

### Sprites (Tile Data starting at 0xB4)

```hex
03 03 03 03 03 03 03 03  03 03 03 03 03 03 03 03
03 03 03 03 03 03 03 03  03 03 03 03 03 03 03 03
03 03 03 03 03 03 03 03  03 03 03 03 03 03 03 03
03 03 03 03 03 03 03 03  03 03 03 03 03 03 03 03
02 02 02 02 02 02 02 02  02 02 02 02 02 02 02 02
02 02 02 02 02 02 02 02  02 02 02 02 02 02 02 02
02 02 02 02 02 02 02 02  02 02 02 02 02 02 02 02
02 02 02 02 02 02 02 02  02 02 02 02 02 02 02 02
01 01 01 01 01 01 01 01  01 01 01 01 01 01 01 01
01 01 01 01 01 01 01 01  01 01 01 01 01 01 01 01
01 01 01 01 01 01 01 01  01 01 01 01 01 01 01 01
01 01 01 01 01 01 01 01  01 01 01 01 01 01 01 01
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
03 00 03 00 03 00 03 00  03 00 00 03 00 03 00 03
00 03 30 00 03 00 03 00  03 00 00 03 00 03 00 03
30 00 03 00 03 00 03 00  00 03 00 03 00 03 30 00
03 00 03 00 03 00 00 03  00 03 00 03 00
```

**Equivalent visual output:**

<img src="./assets/test_sprites.png" width=20/>

## How It Works

1. **File Reading**: Opens the `.GBR` file and reads it into memory
2. **Tile Detection**: Scans from offset `0xB4` to find valid tile data
3. **Empty Tile Detection**: Monitors for 3 consecutive empty tiles (all bytes = 0) to determine where tile data ends
4. **Image Generation**: Converts each 8x8 tile into RGB pixels using the DMG palette
5. **Output**: Writes a PPM (P6 format) image file

## Technical Details

- **Tile Layout**: Each tile is 8x8 pixels (64 bytes total)
- **Tiles Per Row**: Configurable via `TILES_PER_ROW` constant (default: 1)
- **Color Depth**: 2-bit color (4 colors)
- **Output Format**: PPM P6 (binary RGB)
- **Empty Tile Threshold**: 3 consecutive empty tiles

## Requirements

- C compiler (GCC, Clang, MSVC, etc.)
- Standard C library
- (Optional) ImageMagick for PPM to PNG conversion

## Boring stuff

### About .GBR Files

`.GBR` files are created by [Gameboy Tile Designer](https://www.devrs.com/gb/hmgd/gbtd.html), a tool for creating tile-based graphics for Gameboy games. The format stores tile data with a header followed by pixel data for each 8x8 tile.

### License

[Unlicense](./UNLICENSE)