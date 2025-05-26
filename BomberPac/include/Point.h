#pragma once
#include "GameObject.h"
#include "raylib.h"

class Point : public GameObject {
private:
    Vector2 position;
    bool collected;
public:
    Point(int x, int y);
    void Update() override;
    void Draw() override;
    bool IsCollected() const;
    void Collect();
};
