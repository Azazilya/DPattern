#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SCREEN_WIDTH 840
#define SCREEN_HEIGHT 630
#define OBSTACLE_SIZE 30
#define PLAYER_SIZE 20
#define ENEMY_COUNT 4

typedef struct Enemy {
    Vector2 position;
    Color color;
    float size;
} Enemy;

// Peta Pac-Man diperbesar (21x21)
const char* grid[] = {
    "#####################",
    "#.........#.........#",
    "#.###.###.#.###.###.#",
    "#...................#",
    "#.###.#.#####.#.###.#",
    "#.....#...#...#.....#",
    "#####.###.#.###.#####",
    "#...................#",
    "#.###.#.#####.#.###.#",
    "#.#...#...#...#...#.#",
    "#.#.#.###.#.###.#.#.#",
    "#.....#...#...#.....#",
    "###.#.#.#####.#.#.###",
    "#...#.#...#...#.#...#",
    "#.#####.#.#.#.#.#####",
    "#.......#...#.......#",
    "#.###.###.#.###.###.#",
    "#.#.......#.......#.#",
    "#.#.#.#####.#####.#.#",
    "#.....#.......#.....#",
    "#####################"
};
const int GRID_WIDTH = 21;
const int GRID_HEIGHT = 21;

// Posisi grid ke tengah pixel
Vector2 GridPosToPixelCenter(Vector2 gridPos) {
    return (Vector2){
        gridPos.x * OBSTACLE_SIZE + OBSTACLE_SIZE / 2,
        gridPos.y * OBSTACLE_SIZE + OBSTACLE_SIZE / 2
    };
}

// Posisi pixel ke grid
Vector2 PixelPosToGridPos(Vector2 pos) {
    return (Vector2){(int)(pos.x / OBSTACLE_SIZE), (int)(pos.y / OBSTACLE_SIZE)};
}

// Dapatkan posisi bebas secara acak dari grid
Vector2 GetRandomFreeGridPos() {
    while (true) {
        int x = rand() % GRID_WIDTH;
        int y = rand() % GRID_HEIGHT;
        if (grid[y][x] == '.') return GridPosToPixelCenter((Vector2){x, y});
    }
}

// Buat enemy dengan warna dan posisi acak
Enemy CreateEnemy(int type) {
    Enemy enemy;
    enemy.size = PLAYER_SIZE;
    enemy.color = (Color[]){BLUE, RED, GREEN, PURPLE}[type % 4];
    enemy.position = GetRandomFreeGridPos();
    return enemy;
}

// Pergerakan enemy menuju pemain dengan kecepatan yang lebih lambat
void MoveEnemyTowardsPlayer(Enemy* enemy, Vector2 playerPos, int enemyIndex, int frameCounter[]) {
    if (frameCounter[enemyIndex] < 20) { // Memperlambat musuh
        frameCounter[enemyIndex]++;
        return;
    }
    frameCounter[enemyIndex] = 0;

    Vector2 gridPos = PixelPosToGridPos(enemy->position);
    Vector2 playerGridPos = PixelPosToGridPos(playerPos);
    Vector2 bestMove = gridPos;

    // Tentukan arah optimal
    if (playerGridPos.x > gridPos.x) bestMove.x++;
    else if (playerGridPos.x < gridPos.x) bestMove.x--;

    if (playerGridPos.y > gridPos.y) bestMove.y++;
    else if (playerGridPos.y < gridPos.y) bestMove.y--;

    // Periksa apakah langkah tidak menabrak dinding
    if (grid[(int)bestMove.y][(int)bestMove.x] == '.') {
        enemy->position = GridPosToPixelCenter(bestMove);
    }
}

int main() {
    srand(time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bomberpac - Expanded Pacman Map");

    Vector2 playerPos = GetRandomFreeGridPos();
    Enemy enemies[ENEMY_COUNT];
    for (int i = 0; i < ENEMY_COUNT; i++) enemies[i] = CreateEnemy(i);
    int enemyFrameCounter[ENEMY_COUNT] = {0};
    int playerFrameCounter = 0;

    int score = 0;
    bool gameRunning = true;
    Font font = GetFontDefault();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) gameRunning = !gameRunning;
        if (IsKeyPressed(KEY_ESCAPE)) break;

        if (gameRunning) {
            if (playerFrameCounter < 5) { // Mempercepat pemain
                playerFrameCounter++;
            } else {
                Vector2 playerGridPos = PixelPosToGridPos(playerPos);
                Vector2 targetGridPos = playerGridPos;

                if (IsKeyDown(KEY_RIGHT)) targetGridPos.x++;
                if (IsKeyDown(KEY_LEFT))  targetGridPos.x--;
                if (IsKeyDown(KEY_UP))    targetGridPos.y--;
                if (IsKeyDown(KEY_DOWN))  targetGridPos.y++;

                if (targetGridPos.x >= 0 && targetGridPos.x < GRID_WIDTH &&
                    targetGridPos.y >= 0 && targetGridPos.y < GRID_HEIGHT &&
                    grid[(int)targetGridPos.y][(int)targetGridPos.x] == '.') {
                    playerPos = GridPosToPixelCenter(targetGridPos);
                    score++;
                }
                playerFrameCounter = 0;
            }

            for (int i = 0; i < ENEMY_COUNT; i++)
                MoveEnemyTowardsPlayer(&enemies[i], playerPos, i, enemyFrameCounter);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (grid[y][x] == '#') {
                    Rectangle wall = {x * OBSTACLE_SIZE, y * OBSTACLE_SIZE, OBSTACLE_SIZE, OBSTACLE_SIZE};
                    DrawRectangleRec(wall, (Color){10, 10, 100, 255});
                    DrawRectangleLinesEx(wall, 1, (Color){40, 40, 200, 255});
                } else if (grid[y][x] == '.') {
                    DrawCircle(x * OBSTACLE_SIZE + OBSTACLE_SIZE / 2,
                               y * OBSTACLE_SIZE + OBSTACLE_SIZE / 2, 3, ORANGE);
                }
            }
        }

        DrawCircleSector(playerPos, PLAYER_SIZE / 2, 45, 315, 24, YELLOW);
        DrawCircleSector(playerPos, PLAYER_SIZE / 2 - 3, 45, 315, 24, BLACK);

        for (int i = 0; i < ENEMY_COUNT; i++) {
            DrawCircleV(enemies[i].position, enemies[i].size / 2, enemies[i].color);
        }

        DrawRectangle(0, 0, SCREEN_WIDTH, 30, (Color){0, 0, 0, 200});
        DrawTextEx(font, TextFormat("Score: %04d", score), (Vector2){10, 5}, 20, 2, YELLOW);
        DrawText("ARROW KEYS: Move  SPACE: Pause  ESC: Quit", SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT - 30, 20, LIGHTGRAY);

        if (!gameRunning) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 100});
            DrawText("PAUSED", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 15, 30, RAYWHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
