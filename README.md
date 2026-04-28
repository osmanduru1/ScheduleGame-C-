# ScheduleGame

**Authors:** Osman and Suyash and Ibrahim
**Date:** April 2026  
**Course:** SOFTWARE SYSTEMS DEVELOPMENT W/LAB

---

## Overview

ScheduleGame is a schedule-based life simulation game written in C++ with a Qt 6 GUI. Players design a realistic weekly routine — filling each day with activities like classes, study sessions, work shifts, meals, sleep, and breaks — then "live through" the schedule in a turn-based simulation.

Each activity drains or restores five vital stats: **Health**, **Energy**, **Attention**, **Stress**, and **Sleep**. As the simulation advances, random events and interactive decision cards inject unpredictability and player agency. The goal is to survive the entire schedule without any stat hitting a failure threshold.

---

## Features

1. **Weekly Schedule Builder** — A 7-day grid (Mon–Sun) where players add, view, and delete time-blocked activities.
2. **Stat-Driven Simulation** — Deterministic, per-hour stat changes for every activity type.
3. **Random Events** — Passive events that fire automatically (~60% chance per activity) with Common / Rare / Legendary rarity tiers.
4. **Decision Cards** — Interactive swipe-style cards that appear with ~15% probability, pausing simulation and forcing a choice with branching stat outcomes.
5. **End-of-Run Report** — Scored schedule rating (Excellent / Good / Risky / Unsustainable) with personalised improvement suggestions.

---

## Prerequisites

| Requirement | Version |
|---|---|
| C++ Standard | C++17 or later |
| CMake | 3.16 or later |
| Qt | 6.x (Widgets module) |
| Compiler | GCC / Clang / MSVC with C++17 support |

---

## Building

```bash
# 1. Clone the repository
git clone <your-repo-url>
cd ScheduleGame-C-

# 2. Create a build directory and configure
mkdir build && cd build
cmake ..

# 3. Compile
make

# 4. Run
./ScheduleGame
```

> **macOS / Homebrew Qt:** If CMake cannot find Qt6, set the prefix path:
> ```bash
> cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt6)
> ```

---

## Project Structure

```
ScheduleGame-C-/
├── Main.cpp                          # Entry point — creates QApplication and main window
│
├── controllers/
│   ├── ScheduleBuilderWindow.h/.cpp  # Schedule editor UI (7-day week grid)
│   └── SimulationWindow.h/.cpp       # Turn-based simulation UI
│
├── engine/
│   ├── SimulationEngine.h/.cpp       # Core game logic (stat math, event selection)
│   ├── DecisionCard.h/.cpp           # Draggable swipe-card widget
│   ├── DecisionEvent.h               # DecisionEvent and DecisionOption structs
│   └── RandomEvent.h                 # RandomEvent struct and Rarity enum
│
├── models/
│   ├── Activity.h/.cpp               # Single time-blocked schedule entry
│   ├── Schedule.h/.cpp               # Ordered list of Activities
│   └── Stats.h/.cpp                  # Five-stat player model
│
├── MainWindow/
│   ├── schedule_builder.ui           # Qt Designer UI for the builder window
│   └── simulation.ui                 # Qt Designer UI for the simulation window
│
├── CMakeLists.txt                    # Build configuration
└── README.md                         # This file
```

---

## How to Play

### Step 1 — Build Your Schedule

1. Launch the application. The **Schedule Builder** window opens.
2. Use the **Day** dropdown to select which day you are editing (Monday–Sunday).
3. Fill in an **Activity Name**, choose an **Activity Type** from the combo box, and select **Start Time** and **End Time**.
4. Click **Add Activity**. The activity appears in the weekly grid under the correct day column.
5. Click any filled cell to select an activity, then press **Delete Selected Activity** to remove it.
6. The **Remaining Hours** label shows how many unscheduled hours remain for the selected day.
7. Repeat across all days. When ready, click **Start Simulation**.

### Step 2 — Run the Simulation

1. The **Simulation Window** shows five progress bars (Health, Energy, Attention, Stress, Sleep) and the current activity.
2. Press **Next Activity** to process one activity at a time.
3. On each press:
   - Base stat changes are applied (deterministic, scales with duration).
   - A **Decision Card** may appear (~15% chance). Drag it **left** or **right** to choose your response. The "Next Activity" button is locked until you decide.
   - If no decision fires, a **Random Event** may occur (~60% chance). An info dialog shows the event description.
4. If any failure condition is triggered (a resource stat hits 0, or Stress reaches 100), the simulation ends with a loss summary.
5. If all activities are completed successfully, a win summary is displayed with a schedule rating and personalised suggestions.
6. Press **Restart** to close the simulation and return to the builder.

---

## Activity Types and Stat Effects

Each activity applies per-hour effects. Duration = `endHour − startHour`.

| Type | Health | Energy | Attention | Stress | Sleep |
|---|---|---|---|---|---|
| Class | — | −5/hr | −7/hr | +5/hr | — |
| Study | — | −4/hr | −5/hr | +4/hr | — |
| Work | — | −6/hr | — | +6/hr | — |
| Meal | +6/hr | +4/hr | — | — | — |
| Sleep | — | +10/hr | — | — | +12/hr |
| Exercise | +5/hr | −4/hr | — | −3/hr | — |
| Break | — | +3/hr | +3/hr | −4/hr | — |

---

## Stats and Failure Conditions

| Stat | Starting Value | Failure Condition |
|---|---|---|
| Health | 70 | Reaches 0 |
| Energy | 70 | Reaches 0 |
| Attention | 70 | Reaches 0 |
| Stress | 30 | Reaches 100 |
| Sleep | 70 | Reaches 0 |

All stats are clamped to [0, 100]. A simulation ends immediately on any failure condition.

---

## Schedule Rating

At the end of a successful (or failed) run, a score is calculated:

```
score = health + energy + attention + sleep − stress
```

| Score | Rating |
|---|---|
| > 300 | Excellent schedule |
| > 200 | Good schedule |
| > 150 | Risky schedule |
| ≤ 150 | Unsustainable schedule |

---

## Changelog

### v1.0 — Initial Release (April 9, 2026)
- Schedule builder with single-day 24-hour grid
- Core simulation engine with base stat effects
- Random event system with rarity tiers
- Decision card swipe mechanic
- End-of-run summary and rating system

### v1.1 — Prototype Feedback Update (April 10, 2026)
- Added delete-activity functionality
- Improved table selection to auto-fill start/end time dropdowns
- Fixed stat clamping edge cases in Sleep and Exercise activities

### v1.2 — Week Mode (April 26, 2026)
- **Breaking change:** Schedule builder upgraded from single-day to full 7-day week view
- Each day now has an independent Schedule; all days are merged for simulation
- Day selector combo box added; remaining-hours label now shows the selected day
- Clicking a table cell automatically switches the active day
- Simulation combines all days into a single activity sequence

---

## UML Diagram

See `UML.md` in this repository for the full class diagram.

