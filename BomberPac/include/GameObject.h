#pragma once
class GameObject {
public:
    int x, y;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual ~GameObject() {}
};
