#pragma once
#include <QString>
#include "RandomEvent.h"

/*
 * DecisionOption
 *
 * Represents a single choice available to the user during a decision event.
 *
 * Each option contains:
 *  - Display text (what the user sees)
 *  - Stat modifications applied if selected
 *
 * Stat Effects:
 *  - health, energy, attention, stress, sleep
 *  - Values can be positive or negative
 */
struct DecisionOption
{
    QString text;   // Description shown to the user

    int health = 0;     // Change in health
    int energy = 0;     // Change in energy
    int attention = 0;  // Change in attention
    int stress = 0;     // Change in stress
    int sleep = 0;      // Change in sleep
};

/*
 * DecisionEvent
 *
 * Represents an interactive event triggered during simulation.
 *
 * Structure:
 *  - A scenario description is presented to the user
 *  - Two choices are available (left/right)
 *  - Each choice produces different stat effects
 *
 * Additional Properties:
 *  - weight: controls how frequently this event appears
 *  - rarity: indicates how common or rare the event is
 */
struct DecisionEvent
{
    QString description;          // Main scenario text

    DecisionOption leftOption;    // Option selected by swiping left
    DecisionOption rightOption;   // Option selected by swiping right

    int weight;                  // Probability weight for event selection
    Rarity rarity;               // Rarity classification (from RandomEvent.h)
};