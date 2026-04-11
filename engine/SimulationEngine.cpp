#include "SimulationEngine.h"

void SimulationEngine::runActivity(Stats& stats, const Activity& activity)
{
    int h = activity.endHour - activity.startHour;

    switch (activity.type)
    {
        case ActivityType::Class:
            stats.energy -= 5 * h;
            stats.attention -= 7 * h;
            stats.stress += 5 * h;
            break;

        case ActivityType::Study:
            stats.energy -= 4 * h;
            stats.attention -= 5 * h;
            stats.stress += 4 * h;
            break;

        case ActivityType::Work:
            stats.energy -= 6 * h;
            stats.stress += 6 * h;
            break;

        case ActivityType::Meal:
            stats.health += 6 * h;
            stats.energy += 4 * h;
            break;

        case ActivityType::Sleep:
            stats.energy += 10 * h;
            stats.sleep += 12 * h;
            break;

        case ActivityType::Exercise:
            stats.health += 5 * h;
            stats.energy -= 4 * h;
            stats.stress -= 3 * h;
            break;

        case ActivityType::Break:
            stats.energy += 3 * h;
            stats.attention += 3 * h;
            stats.stress -= 4 * h;
            break;
    }

    stats.clamp();
}