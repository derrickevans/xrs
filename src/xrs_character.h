#ifndef XRS_CHARACTER_H
#define XRS_CHARACTER_H

#include "raylib.h"

typedef enum {
    IDLE,
    WALKING,
    RUNNING
} xrs_character_state;

typedef struct {
    Vector2 pos;                // True position of the character
    Vector2 sprite_pos;         // Tweening position of the sprite for smooth walk look
    Texture2D sprite;
    xrs_character_state state;
    bool true_tile_enabled;
} xrs_character_t;

void xrs_render_character(xrs_character_t *character);

#endif // XRS_CHARACTER_H
