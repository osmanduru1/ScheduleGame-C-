#include "SimulationEngine.h"
#include "RandomEvent.h"

#include <vector>
#include <cstdlib>

#include <map>
#include <vector>
#include "RandomEvent.h"

std::map<ActivityType, std::vector<RandomEvent>> events =
{
    {ActivityType::Class,
    {
        {"Surprise quiz!", 0,0,-6,12,0,8},
        {"Interesting lecture!", 0,0,10,0,0,6},
        {"You doze off.", 0,0,-10,0,0,5}
    }},

    {ActivityType::Study,
    {
        {"Productive session!", 0,0,8,0,0,7},
        {"Phone distraction.", 0,0,-8,0,0,6},
        {"You understand everything!", 0,0,0,-5,0,5}
    }},

    {ActivityType::Work,
    {
        {"Difficult customer!", 0,0,0,10,0,7},
        {"Easy shift.", 0,0,0,-5,0,6}
    }},

    {ActivityType::Sleep,
    {
        {"Deep sleep!", 0,10,0,0,5,6},
        {"Nightmare!", 0,0,0,8,0,4}
    }},

    {ActivityType::Break,
    {
        {"Coffee break! Energy +10", 0,10,0,0,0,7},
        {"Relaxing moment. Stress -5", 0,0,0,-5,0,6}
    }}
};
QString SimulationEngine::runRandomEvent(Stats& stats, const Activity& activity)
{
    auto it = events.find(activity.type);

    if (it == events.end())
        return "";

    const std::vector<RandomEvent>& eventList = it->second;

    if (eventList.empty())
        return "";

    // 50% chance an event happens at all
    int trigger = rand() % 100;
    if (trigger > 50)
        return "";

    // weighted random selection
    int totalWeight = 0;
    for (const auto& e : eventList)
        totalWeight += e.probability;

    int roll = rand() % totalWeight;

    int cumulative = 0;

    for (const auto& e : eventList)
    {
        cumulative += e.probability;

        if (roll < cumulative)
        {
            stats.health += e.health;
            stats.energy += e.energy;
            stats.attention += e.attention;
            stats.stress += e.stress;
            stats.sleep += e.sleep;

            stats.clamp();

            return e.description;
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