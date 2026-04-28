#pragma once

/*
 * Stats
 *
 * Represents the current state of the user during simulation.
 *
 * Purpose:
 *  - Tracks key attributes affected by activities, decisions, and events
 *  - Determines whether the simulation continues or ends
 *
 * Attributes:
 *  - health: overall physical condition
 *  - energy: ability to perform tasks
 *  - attention: focus and mental clarity
 *  - stress: accumulated pressure (higher is worse)
 *  - sleep: rest level
 *
 * Value Range:
 *  - All stats are maintained between 0 and 100
 *  - Enforced using clamp()
 *
 * Simulation Rules:
 *  - The simulation ends if:
 *      • any of health, energy, attention, or sleep reaches 0
 *      • stress reaches 100
 */
class Stats {
public:

    int health;     // Physical well-being
    int energy;     // Available energy level
    int attention;  // Focus and concentration
    int stress;     // Stress level (higher = worse)
    int sleep;      // Rest level

    /*
     * Constructor
     *
     * Initializes default starting values for all stats.
     */
    Stats();

    /*
     * Clamps all stat values to the valid range [0, 100].
     *
     * Prevents invalid or overflow values after updates.
     */
    void clamp();

    /*
     * Checks whether the player is still "alive" in the simulation.
     *
     * Returns:
     *  - true if all survival conditions are met
     *  - false if any critical stat reaches a failure threshold
     */
    bool isAlive() const;
};