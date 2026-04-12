#include "SimulationEngine.h"
#include "RandomEvent.h"

#include <vector>
#include <cstdlib>


std::vector<RandomEvent> events =
{
    {"Coffee break! Energy +10", 0, 10, 0, 0, 0, 10},

    {"Unexpected assignment. Stress +10", 0, 0, 0, 10, 0, 10},

    {"Quick nap. Energy +8 Sleep +5", 0, 8, 0, 0, 5, 8},

    {"Phone distraction. Attention -8", 0, 0, -8, 0, 0, 8},

    {"Workout motivation! Health +5 Stress -3", 5, 0, 0, -3, 0, 6}
};


QString SimulationEngine::runRandomEvent(Stats& stats)
{
    int roll = rand() % 100;

    int cumulative = 0;

    for (const RandomEvent& event : events)
    {
        cumulative += event.probability;

        if (roll < cumulative)
        {
            stats.health += event.health;
            stats.energy += event.energy;
            stats.attention += event.attention;
            stats.stress += event.stress;
            stats.sleep += event.sleep;

            stats.clamp();

            return event.description;
        }
    }

    return "";
}

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