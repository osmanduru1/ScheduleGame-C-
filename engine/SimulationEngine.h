#pragma once

#include "../models/Schedule.h"
#include "../models/Stats.h"
#include "RandomEvent.h"
#include "DecisionEvent.h"

#include <QString>

/*
 * SimulationEngine
 *
 * Core logic component responsible for running the simulation.
 *
 * Responsibilities:
 *  - Apply base effects of activities to user stats
 *  - Trigger and process random events
 *  - Select and handle decision events
 *  - Update stats consistently across all interactions
 *
 * Design:
 *  - Acts as the central processing unit for the simulation
 *  - Separates logic from UI (SimulationWindow)
 *  - Uses Activity, RandomEvent, and DecisionEvent data models
 *
 * Usage Flow:
 *  1. runActivity() → applies base activity effects
 *  2. getDecisionEvent() → optionally returns a decision event
 *  3. applyDecision() → applies user-selected outcome
 *  4. runRandomEvent() → applies automatic event effects
 */
class SimulationEngine
{
public:

    /*
     * Applies base effects of an activity.
     *
     * Effects depend on:
     *  - Activity type (e.g., Class, Sleep, Work)
     *  - Duration of the activity
     *
     * Updates stats directly.
     */
    void runActivity(Stats& stats, const Activity& activity);

    /*
     * Executes a random event for the given activity.
     *
     * Behavior:
     *  - May return no event (empty string)
     *  - If triggered, applies stat changes
     *  - Returns description of the event
     */
    QString runRandomEvent(Stats& stats, const Activity& activity);

    /*
     * Determines whether a decision event occurs.
     *
     * Returns:
     *  - Pointer to a DecisionEvent if triggered
     *  - nullptr if no decision event occurs
     */
    DecisionEvent* getDecisionEvent(const Activity& activity);

    /*
     * Applies the effects of a chosen decision option.
     *
     * Updates all relevant stats and clamps values.
     */
    void applyDecision(Stats& stats, const DecisionOption& option);
};