#pragma once
#include <vector>
#include "Observer.h"

class Subject {
protected:
   std::vector<Observer*> observers;
public:
    void Attach(Observer* obs) { observers.push_back(obs); }
    void Notify(int score) {
        for (auto o : observers) o->OnNotify(score);
    }
};
