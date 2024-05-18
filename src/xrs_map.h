#ifndef XRS_MAP_H
#define XRS_MAP_H

#include "raylib.h"

// Returns a Vector two of x, y indices from the pixel x, y coordinate.
Vector2 xrs_pixel_to_cell(int pixel_x, int pixel_y);

// Returns the pixel coordinate from the x, y indices.
Vector2 xrs_cell_to_pixel(int index_x, int index_y);

#endif // XRS_MAP_H