#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Bomb.h"
#include "Point.h"

enum ObjectType { ENEMY, POINT, BOMB };

class GameObjectFactory {
public:
    static GameObject* create(ObjectType type, int x, int y);
};
