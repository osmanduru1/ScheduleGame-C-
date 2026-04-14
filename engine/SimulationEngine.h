#pragma once

#include "../models/Schedule.h"
#include "../models/Stats.h"
#include "RandomEvent.h"
#include "DecisionEvent.h"

#include <QString>

class SimulationEngine
{
public:
    void runActivity(Stats& stats, const Activity& activity);

    QString runRandomEvent(Stats& stats, const Activity& activity);

    DecisionEvent* getDecisionEvent(const Activity& activity);
    void applyDecision(Stats& stats, const DecisionOption& option);
};