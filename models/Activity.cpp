/*
 * Activity.cpp
 *
 * This file implements the Activity class constructor.
 *
 * Purpose:
 *  - Represents a single scheduled activity in the system
 *  - Stores basic information required for both scheduling and simulation
 *
 * Data Stored:
 *  - name: user-defined label (e.g., "Math Class", "Gym")
 *  - type: category of activity (affects simulation behavior)
 *  - startHour: beginning of activity (0–23)
 *  - endHour: end of activity (0–24)
 */

#include "Activity.h"

/*
 * Constructor
 *
 * Initializes an Activity object with:
 *  - name
 *  - type
 *  - start time
 *  - end time
 *
 * Assumptions:
 *  - startHour < endHour
 *  - values are validated before construction (handled in UI layer)
 */
Activity::Activity(std::string name, ActivityType type, int startHour, int endHour)
{
    this->name = name;
    this->type = type;
    this->startHour = startHour;
    this->endHour = endHour;
}