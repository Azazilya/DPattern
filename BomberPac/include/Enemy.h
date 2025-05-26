#pragma once
#include "GameObject.h"
#include "raylib.h"

class Enemy : public GameObject {
private:
    Vector2 position;
    float speed;
public:
    Enemy(int x, int y);
    void Update() override;
    void Draw() override;
};