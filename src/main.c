#include "raylib.h"

#include "xrs_character.h"

const int screen_width = 640;
const int screen_height = 640;

Camera2D camera;

xrs_character_t player;

void xrs_init(void) {
    InitWindow(screen_width, screen_height, "eXpress RS");
    SetTargetFPS(60);

    player.pos = (Vector2) {0.0f, 0.0f};
    player.sprite = LoadTexture("res/textures/ordinary_orvil.png");


    camera.offset = (Vector2) {screen_width / 2, screen_height / 2};
    camera.rotation = 0.0f;
    camera.target = player.pos;
    camera.zoom = 1.0f;

}

void xrs_tick_update(void) {
}

void xrs_update(void) {
}

void xrs_render(void) {
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
            DrawTextureV(player.sprite, player.pos, WHITE);
        EndMode2D();
    EndDrawing();
}

void xrs_exit(void) {
    UnloadTexture(player.sprite);
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