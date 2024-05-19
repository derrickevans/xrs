#include "xrs_character.h"

#include "xrs_utils.h"

void xrs_render_character(xrs_character_t *character) {
    DrawTextureV((*character).sprite, (*character).sprite_pos, WHITE);
    if ((*character).true_tile_enabled) {
        DrawRectangleLines((*character).pos.x, (*character).pos.y, CELL_WIDTH, CELL_HEIGHT, XRS_CYAN);
    }
}