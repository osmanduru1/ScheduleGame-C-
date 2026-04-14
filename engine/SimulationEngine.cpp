#include "SimulationEngine.h"
#include "RandomEvent.h"

#include <vector>
#include <cstdlib>

#include <map>
#include <vector>
#include "RandomEvent.h"
#include <QMessageBox>

std::map<ActivityType, std::vector<RandomEvent>> events =
{
    {ActivityType::Class,
    {
        {"Interesting lecture.", 0, 0, 8, 0, 0, 12, Rarity::Common},
        {"You struggle to stay focused.", 0, 0, -6, 0, 0, 10, Rarity::Common},
        {"You forgot about a quiz.", 0, 0, -4, 10, 0, 8, Rarity::Common},

        {"Professor answers your question clearly.", 0, 0, 10, -4, 0, 6, Rarity::Rare},
        {"Class participation goes well.", 0, 0, 6, -3, 0, 5, Rarity::Rare},
        {"You get called on unexpectedly.", 0, 0, 0, 12, 0, 5, Rarity::Rare},

        {"Professor mentions a research opportunity.", 0, 0, 15, -8, 0, 2, Rarity::Legendary}
    }},

    {ActivityType::Study,
    {
        {"Productive study session.", 0, 0, 8, 0, 0, 12, Rarity::Common},
        {"You get distracted by your phone.", 0, 0, -8, 0, 0, 10, Rarity::Common},
        {"You hit mental fatigue.", 0, -5, -4, 0, 0, 9, Rarity::Common},

        {"You finally understand the material.", 0, 0, 6, -6, 0, 6, Rarity::Rare},
        {"You find a great study method.", 0, 0, 10, 0, 0, 5, Rarity::Rare},
        {"You realize an exam is sooner than expected.", 0, 0, 0, 12, 0, 5, Rarity::Rare},

        {"You enter a flow state.", 0, 0, 16, -8, 0, 2, Rarity::Legendary}
    }},

    {ActivityType::Work,
    {
        {"Routine shift.", 0, -4, 0, 0, 0, 12, Rarity::Common},
        {"Difficult customer.", 0, 0, 0, 10, 0, 10, Rarity::Common},
        {"Long stretch on your feet.", -4, -4, 0, 0, 0, 9, Rarity::Common},

        {"A coworker helps you out.", 0, 0, 0, -5, 0, 6, Rarity::Rare},
        {"A slow and easy shift.", 0, 3, 0, -4, 0, 5, Rarity::Rare},
        {"Manager puts extra pressure on you.", 0, 0, 0, 14, 0, 5, Rarity::Rare},

        {"You get praised for your work.", 0, 0, 8, -8, 0, 2, Rarity::Legendary}
    }},

    {ActivityType::Meal,
    {
        {"You eat a proper meal.", 6, 4, 0, 0, 0, 12, Rarity::Common},
        {"You eat too quickly.", 0, 2, 0, 2, 0, 9, Rarity::Common},
        {"The meal is underwhelming.", 2, 0, 0, 0, 0, 8, Rarity::Common},

        {"A really satisfying meal.", 8, 0, 0, -4, 0, 5, Rarity::Rare},
        {"You run into a friend and relax.", 0, 0, 0, -6, 0, 5, Rarity::Rare},

        {"A perfect meal and mental reset.", 10, 6, 0, -6, 0, 2, Rarity::Legendary}
    }},

    {ActivityType::Sleep,
    {
        {"You get solid sleep.", 0, 8, 0, 0, 5, 12, Rarity::Common},
        {"Restless sleep.", 0, 3, 0, 0, 2, 10, Rarity::Common},
        {"You have a nightmare.", 0, 0, 0, 8, 0, 7, Rarity::Common},

        {"You wake up refreshed.", 0, 12, 0, -4, 0, 6, Rarity::Rare},
        {"You oversleep slightly.", 0, 8, -3, 0, 0, 5, Rarity::Rare},

        {"The best sleep you've had in weeks.", 0, 15, 0, -8, 8, 2, Rarity::Legendary}
    }},

    {ActivityType::Exercise,
    {
        {"You get a decent workout.", 6, -4, 0, 0, 0, 12, Rarity::Common},
        {"You feel sluggish today.", 0, -6, 0, 0, 0, 10, Rarity::Common},
        {"Workout clears your mind.", 0, 0, 0, -5, 0, 8, Rarity::Common},

        {"A great workout session.", 8, 0, 0, -6, 0, 6, Rarity::Rare},
        {"You feel some soreness.", -3, -4, 0, 0, 0, 5, Rarity::Rare},

        {"Runner's high.", 10, 4, 0, -8, 0, 2, Rarity::Legendary}
    }},

    {ActivityType::Break,
    {
        {"You take a coffee break.", 0, 10, 0, 0, 0, 12, Rarity::Common},
        {"You scroll your phone too long.", 0, 0, -5, 0, 0, 9, Rarity::Common},
        {"You get a short mental reset.", 0, 0, 0, -4, 0, 10, Rarity::Common},

        {"A very calming break.", 0, 0, 4, -7, 0, 6, Rarity::Rare},
        {"You take a quick nap.", 0, 8, 0, 0, 4, 5, Rarity::Rare},

        {"A perfect reset.", 0, 10, 8, -8, 0, 2, Rarity::Legendary}
    }}
};

int getRarityChance(Rarity r)
{
    switch (r)
    {
        case Rarity::Common: return 50;     // 50% chance
        case Rarity::Rare: return 20;       // 20%
        case Rarity::Legendary: return 5;   // 5%
    }
    return 0;
}


QString SimulationEngine::runRandomEvent(Stats& stats, const Activity& activity)
{
    auto it = events.find(activity.type);

    if (it == events.end())
        return "";

    const std::vector<RandomEvent>& eventList = it->second;

    if (eventList.empty())
        return "";

    // --- Step 1: Maybe no event at all
    if (rand() % 100 < 40)
        return "";

    // --- Step 2: Filter by rarity chance
    std::vector<RandomEvent> validEvents;

    for (const auto& e : eventList)
    {
        int chance = getRarityChance(e.rarity);

        if (rand() % 100 < chance)
            validEvents.push_back(e);
    }

    if (validEvents.empty())
        return "";

    // --- Step 3: Weighted selection
    int totalWeight = 0;
    for (const auto& e : validEvents)
        totalWeight += e.weight;

    int roll = rand() % totalWeight;
    int cumulative = 0;

    for (const auto& e : validEvents)
    {
        cumulative += e.weight;

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