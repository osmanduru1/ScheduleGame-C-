#pragma once
#include <QString>
#include "RandomEvent.h"

struct DecisionOption
{
    QString text;

    int health = 0;
    int energy = 0;
    int attention = 0;
    int stress = 0;
    int sleep = 0;
};

struct DecisionEvent
{
    QString description;

    DecisionOption leftOption;
    DecisionOption rightOption;

    int weight;
    Rarity rarity;
};