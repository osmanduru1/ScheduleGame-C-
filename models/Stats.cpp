/*
 * Stats.cpp
 *
 * This file implements the Stats class.
 *
 * Purpose:
 *  - Represents the player's current condition during simulation
 *  - Tracks key attributes affected by activities and events
 *
 * Stats Tracked:
 *  - health
 *  - energy
 *  - attention
 *  - stress
 *  - sleep
 *
 * These values are updated continuously throughout the simulation
 * and determine whether the player survives or fails.
 */

#include "Stats.h"

/*
 * Constructor
 *
 * Initializes starting stat values.
 *
 * Design Choice:
 *  - Most stats start at a moderate level (70)
 *  - Stress starts lower (30) to give initial buffer
 */
Stats::Stats() {
    health = 70;
    energy = 70;
    attention = 70;
    stress = 30;
    sleep = 70;
}

/*
 * Ensures all stats remain within valid bounds.
 *
 * Rules:
 *  - All stats are clamped between 0 and 100
 *  - Prevents overflow and invalid states
 *
 * Note:
 *  - Called after every stat update (activities, events, decisions)
 */
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

/*
 * Determines whether the player is still "alive" in the simulation.
 *
 * Survival Conditions:
 *  - health > 0
 *  - energy > 0
 *  - attention > 0
 *  - sleep > 0
 *  - stress < 100
 *
 * If any of these fail, the simulation ends.
 */
bool Stats::isAlive() const {
    return health > 0 &&
           energy > 0 &&
           attention > 0 &&
           sleep > 0 &&
           stress < 100;
}