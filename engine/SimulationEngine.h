#pragma once

#include "../models/Schedule.h"
#include "../models/Stats.h"
#include <vector>
#include <QString>
#include "RandomEvent.h"

class SimulationEngine {
public:
    void runActivity(Stats& stats, const Activity& activity);
    QString runRandomEvent(Stats& stats);
};