cmake_minimum_required(VERSION 3.16)

project(ScheduleGame-C-)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

set(CMAKE_AUTOUIC_SEARCH_PATHS MainWindow)

qt_add_executable(ScheduleGame
    Main.cpp

    controllers/ScheduleBuilderWindow.cpp
    controllers/ScheduleBuilderWindow.h
    controllers/SimulationWindow.cpp
    controllers/SimulationWindow.h

    engine/SimulationEngine.cpp
    engine/SimulationEngine.h

    models/Activity.cpp
    models/Activity.h
    models/Schedule.cpp
    models/Schedule.h
    models/Stats.cpp
    models/Stats.h

    MainWindow/schedule_builder.ui
    MainWindow/simulation.ui
)

target_link_libraries(ScheduleGame PRIVATE Qt6::Widgets)