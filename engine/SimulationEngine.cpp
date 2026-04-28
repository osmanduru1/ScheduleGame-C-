/*
 * SimulationEngine.cpp
 *
 * This file implements the core simulation logic.
 *
 * Responsibilities:
 *  - Determine when decision events occur
 *  - Apply decision outcomes to stats
 *  - Trigger and process random events
 *  - Apply base effects of activities
 *
 * Design:
 *  - Uses weighted random selection for events
 *  - Separates decision events (interactive) and random events (automatic)
 *  - All stat changes are applied through a centralized system
 */

#include "SimulationEngine.h"
#include "RandomEvent.h"

#include <vector>
#include <cstdlib>
#include <map>
#include <QMessageBox>

/*
 * Maps each ActivityType to a list of possible decision events.
 *
 * Each event includes:
 *  - Description
 *  - Two options (left/right)
 *  - Weight (selection probability)
 *  - Rarity
 */
std::map<ActivityType, std::vector<DecisionEvent>> decisionEvents =
{
    {ActivityType::Class,
    {
        {
            "You're feeling tired in class.",
            {"Zone out", 0, 2, -8, -2, 0},
            {"Force yourself to focus", 0, -3, 8, 4, 0},
            10, Rarity::Common
        },

        {
            "The professor asks a question.",
            {"Stay quiet", 0, 0, -2, -2, 0},
            {"Answer confidently", 0, -2, 6, 6, 0},
            8, Rarity::Rare
        },

        {
            "You forgot to review the material.",
            {"Hope for the best", 0, 0, -6, 2, 0},
            {"Quickly cram", 0, -4, 5, 6, 0},
            7, Rarity::Common
        }
    }},

    {ActivityType::Study,
    {
        {
            "You feel distracted.",
            {"Scroll your phone", 0, 0, -8, -3, 0},
            {"Push through", 0, -4, 8, 5, 0},
            10, Rarity::Common
        },

        {
            "You're mentally exhausted.",
            {"Take a break", 0, 6, 0, -5, 2},
            {"Keep studying", 0, -6, 6, 7, 0},
            8, Rarity::Rare
        },

        {
            "You find a hard problem.",
            {"Skip it", 0, 0, -3, -2, 0},
            {"Solve it carefully", 0, -3, 10, 6, 0},
            7, Rarity::Common
        },

        {
            "You're in the zone.",
            {"Keep going", 0, -5, 12, 5, 0},
            {"Stop before burnout", 0, 4, 6, -4, 0},
            5, Rarity::Rare
        }
    }},

    {ActivityType::Work,
    {
        {
            "You're getting tired at work.",
            {"Take it easy", 0, 3, 0, -3, 0},
            {"Push harder", 0, -6, 0, 6, 0},
            10, Rarity::Common
        },

        {
            "A coworker asks for help.",
            {"Help them", 0, -3, 2, -4, 0},
            {"Focus on your own work", 0, 0, 0, 3, 0},
            8, Rarity::Common
        },

        {
            "Your manager is watching.",
            {"Work extra hard", 0, -5, 4, 6, 0},
            {"Stay relaxed", 0, 2, 0, -3, 0},
            7, Rarity::Rare
        }
    }},

    {ActivityType::Meal,
    {
        {
            "You're in a rush while eating.",
            {"Eat quickly", 2, 3, 0, 2, 0},
            {"Take your time", 6, 0, 2, -4, 0},
            10, Rarity::Common
        },

        {
            "You feel like overeating.",
            {"Indulge", 5, 4, 0, -2, 0},
            {"Control yourself", 2, 2, 0, 3, 0},
            8, Rarity::Common
        }
    }},

    {ActivityType::Sleep,
    {
        {
            "You can't fall asleep.",
            {"Stay on your phone", 0, 2, -3, 3, -2},
            {"Try to relax", 0, 5, 0, -4, 4},
            10, Rarity::Common
        },

        {
            "You wake up early.",
            {"Go back to sleep", 0, 4, -3, 0, 4},
            {"Get up and start the day", 0, -2, 5, 2, 0},
            8, Rarity::Rare
        }
    }},

    {ActivityType::Exercise,
    {
        {
            "You feel tired before exercising.",
            {"Skip workout", 0, 4, 0, -3, 0},
            {"Push through", 6, -5, 0, -4, 0},
            10, Rarity::Common
        },

        {
            "You have extra energy.",
            {"Go harder", 8, -6, 0, -5, 0},
            {"Keep it light", 4, -2, 0, -2, 0},
            8, Rarity::Rare
        },

        {
            "You feel a slight pain.",
            {"Stop early", -1, 2, 0, -3, 0},
            {"Ignore it", 5, -5, 0, 2, 0},
            7, Rarity::Common
        }
    }},

    {ActivityType::Break,
    {
        {
            "You have some free time.",
            {"Scroll social media", 0, 2, -5, -2, 0},
            {"Relax properly", 0, 5, 2, -5, 0},
            10, Rarity::Common
        },

        {
            "You're thinking about work.",
            {"Keep worrying", 0, 0, 0, 6, 0},
            {"Clear your mind", 0, 3, 2, -6, 0},
            8, Rarity::Common
        },

        {
            "You feel bored.",
            {"Do something productive", 0, -2, 6, 2, 0},
            {"Just chill", 0, 4, 0, -3, 0},
            7, Rarity::Rare
        }
    }}
};


/*
 * Maps each ActivityType to a list of random events.
 *
 * These events:
 *  - Occur automatically (no user input)
 *  - Modify stats directly
 *  - Use weight + rarity for selection
 */
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
/*
 * Converts rarity level into probability threshold.
 *
 * Used to filter which events are eligible before selection.
 */
int rarityChance(Rarity r)
{
    switch (r)
    {
        case Rarity::Common: return 60;
        case Rarity::Rare: return 25;
        case Rarity::Legendary: return 5;
    }
    return 0;
}

/*
 * Selects a decision event for a given activity.
 *
 * Logic:
 *  - 15% chance to trigger a decision event
 *  - If triggered:
 *      • Select based on weighted probability
 *
 * Returns:
 *  - Pointer to selected DecisionEvent
 *  - nullptr if no event is triggered
 */
DecisionEvent* SimulationEngine::getDecisionEvent(const Activity& activity)
{
    auto it = decisionEvents.find(activity.type);

    if (it == decisionEvents.end())
        return nullptr;

    auto& list = it->second;

    if (list.empty())
        return nullptr;

    // Only 15% chance of triggering a decision
    if (rand() % 100 > 15)
        return nullptr;

    int totalWeight = 0;
    for (const auto& e : list)
        totalWeight += e.weight;

    int roll = rand() % totalWeight;
    int cumulative = 0;

    for (auto& e : list)
    {
        cumulative += e.weight;
        if (roll < cumulative)
            return &e;
    }

    return nullptr;
}

/*
 * Applies the selected decision option to the player's stats.
 *
 * Each stat is updated directly, then clamped to valid bounds.
 */
void SimulationEngine::applyDecision(Stats& stats, const DecisionOption& option)
{
    stats.health += option.health;
    stats.energy += option.energy;
    stats.attention += option.attention;
    stats.stress += option.stress;
    stats.sleep += option.sleep;

    stats.clamp();
}

/*
 * Executes a random event for the given activity.
 *
 * Logic:
 *  - 40% chance nothing happens
 *  - Filter events based on rarity probability
 *  - Select event using weighted random selection
 *  - Apply stat changes
 *
 * Returns:
 *  - Event description (if triggered)
 *  - Empty string if no event occurs
 */
QString SimulationEngine::runRandomEvent(Stats& stats, const Activity& activity)
{
    auto it = events.find(activity.type);

    if (it == events.end())
        return "";

    const std::vector<RandomEvent>& list = it->second;

    if (list.empty())
        return "";

    // 40% chance nothing happens
    if (rand() % 100 < 40)
        return "";

    // Filter events based on rarity probability
    std::vector<RandomEvent> valid;

    for (const auto& e : list)
    {
        if (rand() % 100 < rarityChance(e.rarity))
            valid.push_back(e);
    }

    if (valid.empty())
        return "";

    int totalWeight = 0;
    for (const auto& e : valid)
        totalWeight += e.weight;

    int roll = rand() % totalWeight;
    int cumulative = 0;

    for (const auto& e : valid)
    {
        cumulative += e.weight;

        if (roll < cumulative)
        {
            // Apply stat effects
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

/*
 * Applies base effects of an activity over its duration.
 *
 * Duration (in hours) is calculated as:
 *  endHour - startHour
 *
 * Each activity type modifies stats differently.
 */
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