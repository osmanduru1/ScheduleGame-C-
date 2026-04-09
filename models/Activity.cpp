#include "Activity.h"

Activity::Activity(std::string name, ActivityType type, int duration)
{
    this->name = name;
    this->type = type;
    this->duration = duration;
}