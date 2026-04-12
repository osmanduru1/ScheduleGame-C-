#include "SimulationEngine.h"
#include "RandomEvent.h"

#include <vector>
#include <cstdlib>

std::vector<RandomEvent> classEvents =
{
    {"Surprise quiz! Stress +12 Attention -6", 0, 0, -6, 12, 0, 8},
    {"Interesting lecture! Attention +10", 0, 0, 10, 0, 0, 6},
    {"You doze off in class. Attention -10", 0, 0, -10, 0, 0, 6}
};

std::vector<RandomEvent> studyEvents =
{
    {"Productive study session! Attention +8", 0, 0, 8, 0, 0, 7},
    {"You procrastinate on your phone. Attention -8", 0, 0, -8, 0, 0, 7},
    {"You understand the material! Stress -5", 0, 0, 0, -5, 0, 6}
};

std::vector<RandomEvent> workEvents =
{
    {"Difficult customer! Stress +10", 0, 0, 0, 10, 0, 7},
    {"Easy shift today. Stress -5", 0, 0, 0, -5, 0, 6}
};

std::vector<RandomEvent> sleepEvents =
{
    {"Deep sleep! Energy +10", 0, 10, 0, 0, 5, 7},
    {"Nightmare! Stress +8", 0, 0, 0, 8, 0, 6}
};

std::vector<RandomEvent> breakEvents =
{
    {"Coffee break! Energy +10", 0, 10, 0, 0, 0, 7},
    {"Relaxing moment. Stress -5", 0, 0, 0, -5, 0, 6}
};

QString SimulationEngine::runRandomEvent(Stats& stats, const Activity& activity)
{
    std::vector<RandomEvent>* eventList = nullptr;

    switch (activity.type)
    {
        case ActivityType::Class:
            eventList = &classEvents;
            break;

        case ActivityType::Study:
            eventList = &studyEvents;
            break;

        case ActivityType::Work:
            eventList = &workEvents;
            break;

        case ActivityType::Sleep:
            eventList = &sleepEvents;
            break;

        case ActivityType::Break:
            eventList = &breakEvents;
            break;

        default:
            return "";
    }

    if (eventList->empty())
        return "";

    int roll = rand() % 100;
    int cumulative = 0;

    for (const RandomEvent& event : *eventList)
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