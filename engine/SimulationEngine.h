#pragma once

#include "../models/Schedule.h"
#include "../models/Stats.h"

class SimulationEngine {
public:
    void runActivity(Stats& stats, const Activity& activity);
};