#include "Activity.h"

Activity::Activity(std::string name, ActivityType type, int startHour, int endHour)
{
    this->name = name;
    this->type = type;
    this->startHour = startHour;
    this->endHour = endHour;
}