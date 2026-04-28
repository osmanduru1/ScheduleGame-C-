#pragma once
#include <vector>
#include "Activity.h"

/*
 * Schedule
 *
 * Represents a collection of activities.
 *
 * Purpose:
 *  - Stores activities for a given time scope
 *    (e.g., a single day or a combined weekly schedule)
 *
 * Used by:
 *  - ScheduleBuilderWindow → builds schedules from user input
 *  - SimulationWindow → iterates through activities
 *  - SimulationEngine → processes activities sequentially
 *
 * Design Notes:
 *  - This class acts as a simple container
 *  - It does NOT enforce:
 *      • sorting
 *      • overlap validation
 *  - Those responsibilities are handled at the UI level
 */
class Schedule {
public:

    /*
     * List of activities in the schedule.
     *
     * Each Activity includes:
     *  - name
     *  - type
     *  - start/end times
     */
    std::vector<Activity> activities;

    /*
     * Adds an activity to the schedule.
     *
     * Behavior:
     *  - Appends the activity to the list
     *  - No validation or ordering is performed here
     */
    void addActivity(const Activity& activity);
};