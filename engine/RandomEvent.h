#pragma once

#include <QString>

enum class Rarity
{
    Common,
    Rare,
    Legendary
};

struct RandomEvent
{
    QString description;

    int health = 0;
    int energy = 0;
    int attention = 0;
    int stress = 0;
    int sleep = 0;

    int weight;
    Rarity rarity;
};