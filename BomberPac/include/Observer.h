#pragma once
class Observer {
public:
    virtual void OnNotify(int) = 0;
    virtual ~Observer() {}
};
