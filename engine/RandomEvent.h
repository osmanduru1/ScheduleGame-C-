#ifndef RANDOM_EVENT_H
#define RANDOM_EVENT_H

#include <QtCore/QString>
struct RandomEvent
{
    QString description;

    int health = 0;
    int energy = 0;
    int attention = 0;
    int stress = 0;
    int sleep = 0;

    int probability;   // percentage chance
};

#endif