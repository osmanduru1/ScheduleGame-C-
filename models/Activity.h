#pragma once
#include <string>

enum class ActivityType {
    Class,
    Study,
    Work,
    Meal,
    Sleep,
    Exercise,
    Break
};

class Activity {
public:
    std::string name;
    ActivityType type;
    int startHour;
    int endHour;

    Activity(std::string name, ActivityType type, int startHour, int endHour);
};