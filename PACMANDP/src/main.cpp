#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 630
#define OBSTACLE_SIZE 30
#define PLAYER_SIZE 20
#define ENEMY_COUNT 3
#define MAX_QUEUE 1000

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

typedef struct Enemy {
    Vector2 position;
    Color color;
    float size;
} Enemy;

typedef struct Node {
    int x, y;
} Node;

Vector2 GridPosToPixelCenter(Vector2 gridPos) {
    return (Vector2){
        gridPos.x * OBSTACLE_SIZE + OBSTACLE_SIZE / 2,
        gridPos.y * OBSTACLE_SIZE + OBSTACLE_SIZE / 2
    };
}

Vector2 PixelPosToGridPos(Vector2 pos) {
    return (Vector2){(int)(pos.x / OBSTACLE_SIZE), (int)(pos.y / OBSTACLE_SIZE)};
}

Vector2 GetRandomFreeGridPos() {
    while (true) {
        int x = rand() % GRID_WIDTH;
        int y = rand() % GRID_HEIGHT;
        if (grid[y][x] == '.') return GridPosToPixelCenter((Vector2){x, y});
    }
}

// Strategy Pattern Enemy
bool BFS(Vector2 start, Vector2 target, Vector2* nextStep) {
    int visited[GRID_HEIGHT][GRID_WIDTH] = {0};
    Node parent[GRID_HEIGHT][GRID_WIDTH];

    Node queue[MAX_QUEUE];
    int front = 0, rear = 0;

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    int sx = (int)start.x;
    int sy = (int)start.y;
    int tx = (int)target.x;
    int ty = (int)target.y;

    queue[rear++] = (Node){sx, sy};
    visited[sy][sx] = 1;

    while (front < rear) {
        Node current = queue[front++];
        if (current.x == tx && current.y == ty) break;

        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT &&
                !visited[ny][nx] && grid[ny][nx] == '.') {
                visited[ny][nx] = 1;
                parent[ny][nx] = current;
                queue[rear++] = (Node){nx, ny};
            }
        }
    }

    if (!visited[ty][tx]) return false;

    Node step = (Node){tx, ty};
    while (!(parent[step.y][step.x].x == sx && parent[step.y][step.x].y == sy)) {
        step = parent[step.y][step.x];
    }

    *nextStep = (Vector2){step.x, step.y};
    return true;
}

//Factory Pattern
Enemy CreateEnemy(int type) {
    Enemy enemy;
    enemy.size = PLAYER_SIZE;
    enemy.color = (Color[]){BLUE, RED, GREEN, PURPLE}[type % 4];
    enemy.position = GetRandomFreeGridPos();
    return enemy;
}

void MoveEnemyOnGrid(Enemy* enemy, int enemyIndex, int frameCounter[], Vector2 playerGridPos) {
    if (frameCounter[enemyIndex] < 70) {    //Semakin besar nilai semakin LAMBAT musuh
        frameCounter[enemyIndex]++;
        return;
    }
    frameCounter[enemyIndex] = 0;

    Vector2 enemyGrid = PixelPosToGridPos(enemy->position);
    Vector2 nextStep;
// Strategy Pattern Player
    if (BFS(enemyGrid, playerGridPos, &nextStep)) {
        enemy->position = GridPosToPixelCenter(nextStep);
    }
}

int main() {
    srand(time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PACMAN");
    SetTargetFPS(60);

    Vector2 playerPos = GetRandomFreeGridPos();
    Enemy enemies[ENEMY_COUNT];
    for (int i = 0; i < ENEMY_COUNT; i++) enemies[i] = CreateEnemy(i);
    int enemyFrameCounter[ENEMY_COUNT] = {0};
    int playerFrameCounter = 0;
//Observer Pattern
    int score = 0;
    int lives = 3;
    bool gameRunning = true;
    Font font = GetFontDefault();

    bool collectedDots[GRID_HEIGHT][GRID_WIDTH] = {false};
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x] == '.') collectedDots[y][x] = true;
        }
    }

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) gameRunning = !gameRunning; //Observer Pattern
        if (IsKeyPressed(KEY_ESCAPE)) break;

        bool allCollected = true;

        if (gameRunning) {
            if (playerFrameCounter < 5) {
                playerFrameCounter++;
            } else {
                Vector2 playerGridPos = PixelPosToGridPos(playerPos);
                Vector2 targetGridPos = playerGridPos;
// Command Pattern
                if (IsKeyDown(KEY_RIGHT)) targetGridPos.x++;
                if (IsKeyDown(KEY_LEFT))  targetGridPos.x--;
                if (IsKeyDown(KEY_UP))    targetGridPos.y--;
                if (IsKeyDown(KEY_DOWN))  targetGridPos.y++;

                if (targetGridPos.x >= 0 && targetGridPos.x < GRID_WIDTH &&
                    targetGridPos.y >= 0 && targetGridPos.y < GRID_HEIGHT &&
                    grid[(int)targetGridPos.y][(int)targetGridPos.x] == '.') {

                    playerPos = GridPosToPixelCenter(targetGridPos);

                    if (collectedDots[(int)targetGridPos.y][(int)targetGridPos.x]) {
                        collectedDots[(int)targetGridPos.y][(int)targetGridPos.x] = false;
                        score++;
                    }
                }
                playerFrameCounter = 0;
            }

            Vector2 playerGrid = PixelPosToGridPos(playerPos);
            for (int i = 0; i < ENEMY_COUNT; i++)
                MoveEnemyOnGrid(&enemies[i], i, enemyFrameCounter, playerGrid);

            for (int i = 0; i < ENEMY_COUNT; i++) {
                if (CheckCollisionCircles(playerPos, PLAYER_SIZE / 2, enemies[i].position, enemies[i].size / 2)) {
                    lives--;
                    if (lives <= 0) gameRunning = false;
                    else playerPos = GetRandomFreeGridPos();
                    break;
                }
            }

            for (int y = 0; y < GRID_HEIGHT && allCollected; y++) {
                for (int x = 0; x < GRID_WIDTH && allCollected; x++) {
                    if (grid[y][x] == '.' && collectedDots[y][x]) allCollected = false;
                }
            }

            if (allCollected) gameRunning = false;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (grid[y][x] == '#') {
                    Rectangle wall = {x * OBSTACLE_SIZE, y * OBSTACLE_SIZE, OBSTACLE_SIZE, OBSTACLE_SIZE};
                    DrawRectangleRec(wall, (Color){10, 10, 100, 255});
                    DrawRectangleLinesEx(wall, 1, (Color){40, 40, 200, 255});
                } else if (grid[y][x] == '.' && collectedDots[y][x]) {
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
        DrawTextEx(font, TextFormat("Lives: %d", lives), (Vector2){200, 5}, 20, 2, RED);
        DrawText("ARROW KEYS: Move  SPACE: Pause  ESC: Quit", SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT - 30, 20, LIGHTGRAY);
// State Pattern
        if (!gameRunning && lives <= 0) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 150});
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 40, RED);
        } else if (!gameRunning && allCollected) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 150});
            DrawText("GAME SELESAI!", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 20, 40, GREEN);
        } else if (!gameRunning) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 100});
            DrawText("PAUSED", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 15, 30, RAYWHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
