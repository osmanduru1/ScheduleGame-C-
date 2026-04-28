#pragma once

#include <QString>

/*
 * Rarity
 *
 * Defines how common or rare an event is.
 *
 * Used by the simulation engine to:
 *  - Categorize events
 *  - Potentially influence selection probability
 */
enum class Rarity
{
    Common,     // Frequently occurring events
    Rare,       // Less frequent events
    Legendary   // Very rare, high-impact events
};

/*
 * RandomEvent
 *
 * Represents a non-interactive event that occurs during simulation.
 *
 * Unlike DecisionEvent:
 *  - No user input is required
 *  - Effects are applied immediately
 *
 * Structure:
 *  - description: text shown to the user
 *  - stat changes: direct impact on user stats
 *  - weight: controls likelihood of occurrence
 *  - rarity: classification of event frequency/importance
 */
struct RandomEvent
{
    QString description;   // Event message displayed to user

    /*
     * Stat modifications applied when event occurs.
     * Values may be positive or negative.
     */
    int health = 0;
    int energy = 0;
    int attention = 0;
    int stress = 0;
    int sleep = 0;

    int weight;    // Probability weight for event selection
    Rarity rarity; // Event rarity category
};