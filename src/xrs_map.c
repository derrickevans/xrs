#include "xrs_map.h"
#include "xrs_utils.h"

Vector2 xrs_pixel_to_cell(int pixel_x, int pixel_y) {
    return (Vector2) {pixel_x / CELL_WIDTH, pixel_y / CELL_HEIGHT};
}

Vector2 xrs_cell_to_pixel(int index_x, int index_y) {
    return (Vector2) {index_x * CELL_WIDTH, index_y * CELL_HEIGHT};
}