#include "raylib.h"

#include "xrs_character.h"

const int screen_width = 640;
const int screen_height = 640;

Camera2D camera;

xrs_character_t player;

#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define CELL_WIDTH 32
#define CELL_HEIGHT 32

Texture2D map[MAP_WIDTH * MAP_HEIGHT];
Texture2D grass, dirt;

void xrs_init(void) {
    InitWindow(screen_width, screen_height, "eXpress RS");
    SetTargetFPS(60);

    player.pos = (Vector2) {9.0f * CELL_WIDTH, 9.0f * CELL_HEIGHT};
    player.sprite = LoadTexture("res/textures/ordinary_orvil.png");
    grass = LoadTexture("res/textures/grass_0001.png");
    dirt = LoadTexture("res/textures/dirt_0001.png");

    camera.offset = (Vector2) {screen_width / 2, screen_height / 2};
    camera.rotation = 0.0f;
    camera.target = player.pos;
    camera.zoom = 1.00f;

    // Generate the map.
    for (int h = 0; h < MAP_HEIGHT; h++) {
        for (int w = 0; w < MAP_WIDTH; w++) {
            int index = w + h * MAP_WIDTH;

            if ((w == 9 && h == 9) || (w == 11 && h == 9)) {
                map[index] = dirt;
            } else {
                map[index] = grass;
            }
        }
    }
}
           
void xrs_tick_update(void) {
}

void xrs_update(void) {
    if (IsKeyPressed(KEY_D)) player.pos.x += 1 * CELL_WIDTH;

    camera.offset = (Vector2) {screen_width / 2, screen_height / 2};
    camera.rotation = 0.0f;
    camera.target = player.pos;
    camera.zoom = 1.00f;
}

void xrs_draw_map(void) {
    Vector2 pos = (Vector2) {0.0f, 0.0f};
    // Draw the map.
    for (int h = 0; h < MAP_HEIGHT; h++) {
        pos.x = 0.0f;
        for (int w = 0; w < MAP_WIDTH; w++) {
            DrawTextureV(map[w + h * MAP_WIDTH], pos, WHITE);
            pos.x += CELL_WIDTH;
        }
        pos.y += CELL_HEIGHT;
    }
}

void xrs_render(void) {
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
            xrs_draw_map();
            DrawTextureV(player.sprite, player.pos, WHITE);
        EndMode2D();
    EndDrawing();
}

void xrs_exit(void) {
    UnloadTexture(player.sprite);
    UnloadTexture(grass);
    UnloadTexture(dirt);
    CloseWindow();
}

int main(void) {
    xrs_init();

    while (!WindowShouldClose()) {
        xrs_update();
        xrs_render();     
    }

    xrs_exit();
    return 0;
}