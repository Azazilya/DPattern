#include <iostream>
#include "raylib.h"

int main() {
    InitWindow(900, 600, "PacBom 2D");
    SetTargetFPS(60);

    Player* player = new Player(100, 100);
    ScoreManager scoreManager;
    player->Attach(&scoreManager);

   std::vector<GameObject*> enemies;
   std::vector<GameObject*> points;

    enemies.push_back(GameObjectFactory::create(ENEMY, 300, 200));
    points.push_back(GameObjectFactory::create(POINT, 400, 300));

    while (!WindowShouldClose()) {
        player->HandleInput();
        player->Update();
        for(auto& e: enemies) e->Update();
        for(auto& p: points) p->Update();

        BeginDrawing();
        ClearBackground(BLACK);
        player->Draw();
        for(auto& e: enemies) e->Draw();
        for(auto& p: points) p->Draw();

        DrawText(TextFormat("Score: %d", scoreManager.GetScore()), 10, 10, 20, YELLOW);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
