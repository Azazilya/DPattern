#pragma once
#include "Observer.h"

class ScoreManager : public Observer {
    int score = 0;
public:
    void OnNotify(int s) override { score += s; }
    int GetScore() const { return score; }
};
