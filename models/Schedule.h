#pragma once
#include <vector>
#include "Activity.h"

class Schedule {
public:
    std::vector<Activity> activities;

    void addActivity(const Activity& activity);
};