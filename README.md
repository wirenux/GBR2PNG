# GBR2PNG

**GBR2PNG** is a lightweight C utility to extract tile data from Gameboy Tile Designer (`.GBR`) files and convert them into a `.PNG`.

## Features

- Automatically identifies the number of tiles by scanning for tile data starting at offset `0xB4`
- Stops processing after finding a sequence of **3 empty tiles** to avoid exporting unnecessary tiles
- Written in standard C with no external library requirements.
- Renders images using the original Gameboy green-scale colors.

## Conversion Process

| Component | Specification |
| ----------- | --------------- |
| Tile Size | `8x8` Pixels |
| Data Offset | `0xB4` (Standard .GBR header length) |
| Palette | 4-color DMG |
| Output Format | PNG |

### Example of a `.GBR` file

#### Header

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

Palettes:

```hex
00 01 02 03  White  Light-Grey  Grey  Black
```

Sprites:

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

Equivalent:

![Test Sprites](./assets/test_sprites.png)
