#pragma once

class Stats {
public:
    int health;
    int energy;
    int attention;
    int stress;
    int sleep;

    Stats();

    void clamp();
    bool isAlive() const;
};