#include "raylib.h"
#include "raymath.h"

#include "xrs_utils.h"
#include "xrs_character.h"
#include "xrs_map.h"

const int screen_width = 640;
const int screen_height = 640;

Camera2D camera;

xrs_character_t player;

Texture2D map[MAP_WIDTH * MAP_HEIGHT];
Texture2D grass, dirt;

Vector2 mouse_pos;
Vector2 mouse_world;

void xrs_init(void) {
    InitWindow(screen_width, screen_height, "eXpress RS");
    SetTargetFPS(60);

    player.pos = xrs_cell_to_pixel(1, 1);
    player.sprite_pos = player.pos;
    player.sprite = LoadTexture("res/textures/ordinary_orvil.png");
    grass = LoadTexture("res/textures/grass_0001.png");
    dirt = LoadTexture("res/textures/dirt_0001.png");
    player.state = IDLE;
    player.true_tile_enabled = true;

    //camera.offset = (Vector2) {screen_width / 2, screen_height / 2};
    camera.offset = (Vector2) {0.0f, 0.0f};
    camera.rotation = 0.0f;
    //camera.target = player.pos;
    camera.target = (Vector2) {0.0f, 0.0f};
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

Vector2 mouse_cell_clicked;
Vector2 player_cell;
Vector2 direction;
float timer = 0.0f;
int speed = 1;
bool is_walking = false;
void xrs_tick_update(void) {
    timer += 1.0f;

    if (player.state == WALKING) {
        player_cell = xrs_pixel_to_cell(player.pos.x, player.pos.y);
        direction = Vector2Subtract(mouse_cell_clicked, player_cell);

        player_cell = Vector2Add(player_cell, direction);
        player.pos = xrs_cell_to_pixel(player_cell.x, player_cell.y);

        player.state = IDLE;
    }
}

void xrs_update(void) {
    mouse_pos = GetMousePosition();
    mouse_world = GetWorldToScreen2D(GetMousePosition(), camera);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        mouse_cell_clicked = xrs_pixel_to_cell(mouse_pos.x, mouse_pos.y);

        player.state = WALKING;
    }

    switch (player.state) {
        case WALKING:
            // Play walking animation
            float x_amount = Lerp(player.sprite_pos.x, (float) (xrs_cell_to_pixel(mouse_cell_clicked.x, mouse_cell_clicked.y)).x, 0.05f);
            float y_amount = Lerp(player.sprite_pos.y, (float) (xrs_cell_to_pixel(mouse_cell_clicked.x, mouse_cell_clicked.y)).y, 0.05f);
            player.sprite_pos.x += x_amount * GetFrameTime();
            player.sprite_pos.y += y_amount * GetFrameTime();
        break;

        case RUNNING:
            // Play running animation
        break;

        case IDLE:
        default:
            // Play idle animation
        break;
    };
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

#define XRS_RENDER_DEBUG 0
void xrs_render_debug(void) {
    DrawText(TextFormat("Tick Time %.2f", timer), 10, 10, 20, BLACK);
    DrawText(TextFormat("Mouse (%.2f, %.2f)", mouse_pos.x, mouse_pos.y), 100, 100, 20, YELLOW);
    //DrawText(TextFormat("Cell  (%d, %d)", (int) cell.x, (int) cell.y), 100, 125, 20, GOLD);
    DrawText(TextFormat("World (%.2f, %.2f)", mouse_world.x, mouse_world.y), 100, 150, 20, ORANGE);
    Vector2 p = xrs_pixel_to_cell(player.pos.x, player.pos.y);
    DrawText(TextFormat("Player(%.2f, %.2f)", p.x, p.y), 100, 175, 20, PINK);
    DrawText(TextFormat("Cell Clicked (%.2f, %.2f)", mouse_cell_clicked.x, mouse_cell_clicked.y), 100, 200, 20, RED);
    DrawText(TextFormat("Direciton (%.2f, %.2f)", direction.x, direction.y), 100, 225, 20, MAROON);
}

void xrs_render(void) {
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
            Vector2 cell = xrs_pixel_to_cell((int) mouse_pos.x, (int) mouse_pos.y);
            Vector2 pixel = xrs_cell_to_pixel(cell.x, cell.y);

            xrs_draw_map();
            xrs_render_character(&player);

#if XRS_RENDER_DEBUG
            xrs_render_debug();
#endif

            DrawRectangleLines(pixel.x, pixel.y, CELL_WIDTH, CELL_HEIGHT, MAGENTA);
        EndMode2D();

    EndDrawing();
}

void xrs_exit(void) {
    UnloadTexture(player.sprite);
    UnloadTexture(grass);
    UnloadTexture(dirt);
    CloseWindow();
}

float tick_timer = 0.0f;
int main(void) {
    xrs_init();

    while (!WindowShouldClose()) {
        // Cap xrs_tick_update() to run x amount of times per second.
        tick_timer += GetFrameTime();
        if (tick_timer >= 1.0f) {
            tick_timer = 0.0f;
            xrs_tick_update();
        }
        // xrs_tick_update();
        xrs_update();
        xrs_render();     
    }

    xrs_exit();
    return 0;
}