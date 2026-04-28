#pragma once
#include <string>

/*
 * ActivityType
 *
 * Defines the category of an activity.
 *
 * Each type influences how the activity affects user stats
 * during simulation (handled in SimulationEngine).
 */
enum class ActivityType {
    Class,     // Academic class (mental effort, stress)
    Study,     // Independent study (focus-intensive)
    Work,      // Job or labor (energy drain, stress)
    Meal,      // Eating (restores health/energy)
    Sleep,     // Rest (restores energy and sleep)
    Exercise,  // Physical activity (health gain, energy cost)
    Break      // Relaxation time (reduces stress)
};

/*
 * Activity
 *
 * Represents a single scheduled activity.
 *
 * Used by:
 *  - ScheduleBuilderWindow (UI input and display)
 *  - SimulationEngine (stat calculations)
 *
 * Structure:
 *  - name: user-defined label for the activity
 *  - type: determines simulation behavior
 *  - startHour / endHour: defines duration (in hours)
 *
 * Notes:
 *  - Time is stored in hours (0–24)
 *  - Duration is calculated as (endHour - startHour)
 */
class Activity {
public:

    std::string name;     // Activity name (e.g., "Gym", "Lecture")
    ActivityType type;    // Category of activity
    int startHour;        // Start time (0–23)
    int endHour;          // End time (1–24)

    /*
     * Constructor
     *
     * Initializes an Activity with:
     *  - name
     *  - type
     *  - start and end times
     *
     * Assumptions:
     *  - startHour < endHour
     *  - Validation is handled by the UI layer
     */
    Activity(std::string name, ActivityType type, int startHour, int endHour);
};