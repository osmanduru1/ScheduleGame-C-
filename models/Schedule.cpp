/*
 * Schedule.cpp
 *
 * This file implements the Schedule class behavior.
 *
 * Purpose:
 *  - Represents a collection of activities for a given time period
 *    (e.g., a single day or a combined weekly schedule)
 *
 * Usage:
 *  - ScheduleBuilderWindow → builds schedules from user input
 *  - SimulationEngine → reads activities sequentially
 *
 * Structure:
 *  - Internally stores a list (vector) of Activity objects
 */

#include "Schedule.h"

/*
 * Adds a new activity to the schedule.
 *
 * Behavior:
 *  - Appends the activity to the internal list
 *  - Does NOT enforce ordering or conflict checking
 *
 * Notes:
 *  - Sorting (by start time) is handled externally (UI layer)
 *  - Validation (e.g., overlapping times) is not enforced here
 */
void Schedule::addActivity(const Activity& activity) {
    activities.push_back(activity);
}