#include "Stats.h"

Stats::Stats() {
    health = 70;
    energy = 70;
    attention = 70;
    stress = 30;
    sleep = 70;
}

void Stats::clamp() {
    if (health < 0) health = 0;
    if (energy < 0) energy = 0;
    if (attention < 0) attention = 0;
    if (sleep < 0) sleep = 0;

    if (health > 100) health = 100;
    if (energy > 100) energy = 100;
    if (attention > 100) attention = 100;
    if (sleep > 100) sleep = 100;

    if (stress < 0) stress = 0;
    if (stress > 100) stress = 100;
}

bool Stats::isAlive() const {
    return health > 0 && energy > 0 && attention > 0 && sleep > 0 && stress < 100;
}