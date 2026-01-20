#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TILE_SIZE 8
#define BYTES_PER_TILE 64
#define TILES_PER_ROW 1
#define EMPTY_TILE_THRESHOLD 3

#define TILE_DATA_OFFSET 0xB4

// PNG
typedef struct {
    uint8_t r, g, b;
} RGB;

// Gameboy DMG palette
RGB palette[4] = {
    {155, 188, 15},  // 0 - Lightest
    {139, 172, 15},  // 1 - Light
    {48, 98, 48},    // 2 - Dark
    {15, 56, 15}     // 3 - Darkest
};

// check if a tile is empty (all pixels are 0)
int is_tile_empty(uint8_t* tile_data) {
    for (int i = 0; i < BYTES_PER_TILE; i++) {
        if (tile_data[i] != 0) {
            return 0;
        }
    }
    return 1;
}

void write_ppm(const char* filename, RGB* image, int width, int height) {
    FILE* f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Error: Cannot create output file %s\n", filename);
        return;
    }

    fprintf(f, "P6\n%d %d\n255\n", width, height);
    fwrite(image, sizeof(RGB), width * height, f);
    fclose(f);
    printf("Saved image to %s (%dx%d)\n", filename, width, height);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        printf("Extracts Gameboy tiles from a .GBR file and saves as .PNG image\n");
        printf("Automatically stops when %d consecutive empty tiles are found\n", EMPTY_TILE_THRESHOLD);
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = "output.png";

    // open input file
    FILE* f = fopen(input_file, "rb");
    if (!f) {
        fprintf(stderr, "Error: Cannot open input file %s\n", input_file);
        return 1;
    }

    // get file size
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // read file into buffer
    uint8_t* file_data = (uint8_t*)malloc(file_size);
    if (!file_data) {
        fprintf(stderr, "Error: Cannot allocate memory\n");
        fclose(f);
        return 1;
    }

    fread(file_data, 1, file_size, f);
    fclose(f);

    // calculate maximum number of tiles
    long available_bytes = file_size - TILE_DATA_OFFSET;
    if (available_bytes < 0) {
        fprintf(stderr, "Error: File too small (offset beyond file size)\n");
        free(file_data);
        return 1;
    }

    int max_tiles = available_bytes / BYTES_PER_TILE;
    printf("File size: %ld bytes\n", file_size);
    printf("Tile data offset: 0x%X\n", TILE_DATA_OFFSET);
    printf("Maximum possible tiles: %d\n", max_tiles);

    if (max_tiles == 0) {
        fprintf(stderr, "Error: No complete tiles found\n");
        free(file_data);
        return 1;
    }

    // detect actual number of tiles by checking for consecutive empty tiles
    int num_tiles = 0;
    int empty_count = 0;
    uint8_t* tile_ptr = file_data + TILE_DATA_OFFSET;

    for (int i = 0; i < max_tiles; i++) {
        if (is_tile_empty(tile_ptr)) {
            empty_count++;
            if (empty_count >= EMPTY_TILE_THRESHOLD) {
                printf("Found %d consecutive empty tiles at tile %d, stopping here\n",
                       EMPTY_TILE_THRESHOLD, i - EMPTY_TILE_THRESHOLD + 1);
                break;
            }
        } else {
            empty_count = 0;
            num_tiles = i + 1;
        }
        tile_ptr += BYTES_PER_TILE;
    }

    printf("Actual number of tiles (excluding trailing empty): %d\n", num_tiles);

    if (num_tiles == 0) {
        fprintf(stderr, "Error: No non-empty tiles found\n");
        free(file_data);
        return 1;
    }

    // calculate output image dimensions
    int tiles_per_row = TILES_PER_ROW;
    int num_rows = (num_tiles + tiles_per_row - 1) / tiles_per_row;
    int img_width = tiles_per_row * TILE_SIZE;
    int img_height = num_rows * TILE_SIZE;

    // allocate image buffer
    RGB* image = (RGB*)calloc(img_width * img_height, sizeof(RGB));
    if (!image) {
        fprintf(stderr, "Error: Cannot allocate image memory\n");
        free(file_data);
        return 1;
    }

    // extract and draw tiles
    tile_ptr = file_data + TILE_DATA_OFFSET;

    for (int tile_idx = 0; tile_idx < num_tiles; tile_idx++) {
        int tile_x = (tile_idx % tiles_per_row) * TILE_SIZE;
        int tile_y = (tile_idx / tiles_per_row) * TILE_SIZE;

        for (int y = 0; y < TILE_SIZE; y++) {
            for (int x = 0; x < TILE_SIZE; x++) {
                uint8_t pixel_value = tile_ptr[y * TILE_SIZE + x];
                int img_x = tile_x + x;
                int img_y = tile_y + y;

                image[img_y * img_width + img_x] = palette[pixel_value % 4];
            }
        }

        tile_ptr += BYTES_PER_TILE;
    }

    // write output image
    write_ppm(output_file, image, img_width, img_height);

    // cleanup
    free(image);
    free(file_data);

    return 0;
}