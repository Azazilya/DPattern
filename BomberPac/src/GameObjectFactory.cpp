#include "GameObjectFactory.h"

GameObject* GameObjectFactory::create(ObjectType type, int x, int y) {
    switch(type) {
        case ENEMY: return new Enemy(x, y);
        case POINT: return new Point(x, y);
        case BOMB:  return new Bomb(x, y);
        default:    return nullptr;
    }
}
