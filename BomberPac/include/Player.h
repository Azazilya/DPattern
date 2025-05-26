#pragma once
#include "GameObject.h"
#include "Subject.h"

class Player : public GameObject, public Subject {
public:
    Player(int x, int y);
    void HandleInput();
    void Update() override;
    void Draw() override;
};
