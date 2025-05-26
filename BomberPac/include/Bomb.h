#pragma once
#include "GameObject.h"
#include "raylib.h"

class Bomb : public GameObject {
private:
    Vector2 position;
    float timer;
    bool exploded;
public:
    Bomb(int x, int y);
    void Update() override;
    void Draw() override;
    bool HasExploded() const;
};
