/*
 * SimulationWindow.cpp
 *
 * This file handles the simulation phase of the application.
 *
 * Purpose:
 *  - Simulate the user's schedule activity-by-activity
 *  - Update player stats (health, energy, stress, etc.)
 *  - Introduce decision events and random events
 *  - Provide a final evaluation of the schedule
 *
 * Simulation Flow:
 *  - Activities are processed sequentially
 *  - Each activity modifies stats via SimulationEngine
 *  - Decision events allow user interaction
 *  - Random events introduce variability
 *  - Simulation ends when:
 *      • All activities are completed, or
 *      • Stats drop below survival threshold
 */

#include "SimulationWindow.h"
#include "ui_simulation.h"
#include "../engine/DecisionCard.h"

#include <cstdlib>
#include <ctime>
#include <QMessageBox>

/*
 * Constructor
 *
 * Initializes:
 *  - UI elements
 *  - Stat progress bars
 *  - Random seed for events
 *  - First activity display
 */
SimulationWindow::SimulationWindow(const Schedule& schedule, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::SimulationWindow),
      schedule(schedule),
      currentIndex(0)
{
    ui->setupUi(this);

    // Initialize stat bars (0–100 scale)
    ui->healthBar->setRange(0, 100);
    ui->energyBar->setRange(0, 100);
    ui->attentionBar->setRange(0, 100);
    ui->stressBar->setRange(0, 100);
    ui->sleepBar->setRange(0, 100);

    // Seed randomness for simulation variability
    srand(time(nullptr));

    ui->resultLabel->setText("");

    updateStatsDisplay();
    updateActivityDisplay();

    // Button connections
    connect(ui->nextActivityButton, &QPushButton::clicked,
            this, &SimulationWindow::onNextActivityClicked);

    connect(ui->restartButton, &QPushButton::clicked,
        this, &SimulationWindow::onRestartClicked);
}

/*
 * Destructor
 */
SimulationWindow::~SimulationWindow()
{
    delete ui;
}

/*
 * Updates UI progress bars to reflect current stats.
 */
void SimulationWindow::updateStatsDisplay()
{
    ui->healthBar->setValue(stats.health);
    ui->energyBar->setValue(stats.energy);
    ui->attentionBar->setValue(stats.attention);
    ui->stressBar->setValue(stats.stress);
    ui->sleepBar->setValue(stats.sleep);
}

/*
 * Displays the current activity being simulated.
 */
void SimulationWindow::updateActivityDisplay()
{
    if (currentIndex < static_cast<int>(schedule.activities.size())) {

        const Activity& activity = schedule.activities[currentIndex];

        QString text =
            "Current Activity: "
            + QString::fromStdString(activity.name)
            + " (" + QString::number(activity.startHour)
            + ":00 - "
            + QString::number(activity.endHour) + ":00)";

        ui->currentActivityLabel->setText(text);
    }
    else {
        ui->currentActivityLabel->setText("No more activities.");
    }
}

/*
 * Closes the simulation window (restart action).
 */
void SimulationWindow::onRestartClicked()
{
    this->close();
}

/*
 * Finalizes the simulation and generates a report.
 *
 * Includes:
 *  - Final stats
 *  - Overall rating
 *  - Suggestions based on weaknesses
 */
void SimulationWindow::finishSimulation(const QString& message)
{
    ui->nextActivityButton->setEnabled(false);

    // Compute overall score
    int score =
        stats.health +
        stats.energy +
        stats.attention +
        stats.sleep -
        stats.stress;

    QString rating;

    if (score > 300)
        rating = "Excellent schedule.";
    else if (score > 200)
        rating = "Good schedule.";
    else if (score > 150)
        rating = "Risky schedule.";
    else
        rating = "Unsustainable schedule.";

    // Generate improvement suggestions
    QString suggestions;

    if (stats.sleep < 40)
        suggestions += "• You should add more sleep.\n";

    if (stats.energy < 40)
        suggestions += "• Your schedule drains too much energy.\n";

    if (stats.stress > 70)
        suggestions += "• Your schedule causes too much stress.\n";

    if (stats.attention < 40)
        suggestions += "• Consider adding breaks.\n";

    if (suggestions.isEmpty())
        suggestions = "No major issues detected.";

    // Build final report
    QString report =
        message + "\n\n"
        "Final Stats\n"
        "Health: " + QString::number(stats.health) + "\n"
        "Energy: " + QString::number(stats.energy) + "\n"
        "Attention: " + QString::number(stats.attention) + "\n"
        "Stress: " + QString::number(stats.stress) + "\n"
        "Sleep: " + QString::number(stats.sleep) + "\n\n"
        "Schedule Rating: " + rating + "\n\n"
        "Suggestions:\n" + suggestions;

    ui->resultLabel->setText(report);
}

/*
 * Advances simulation to the next activity.
 *
 * Handles:
 *  - Applying activity effects
 *  - Triggering decision events (interactive)
 *  - Triggering random events (non-interactive)
 *  - Checking survival conditions
 */
void SimulationWindow::onNextActivityClicked()
{
    // End condition
    if (currentIndex >= static_cast<int>(schedule.activities.size())) {
        finishSimulation("You survived the full schedule. You win!");
        return;
    }

    const Activity& activity = schedule.activities[currentIndex];

    // 1️⃣ Apply base activity effects
    engine.runActivity(stats, activity);
    updateStatsDisplay();

    // 2️⃣ Try decision event
    DecisionEvent* decision = engine.getDecisionEvent(activity);

    if (decision != nullptr)
    {
        DecisionCard* card = new DecisionCard(*decision, this);

        // center the card
        card->move(width()/2 - 150, height()/2 - 100);
        card->show();

        // disable button while choosing
        ui->nextActivityButton->setEnabled(false);

        connect(card, &DecisionCard::decisionMade, this,
        [this, card, activity](const DecisionOption& option)
        {
            // Apply decision
            engine.applyDecision(stats, option);
            updateStatsDisplay();

            card->deleteLater();
            ui->nextActivityButton->setEnabled(true);

            // 👉 AFTER decision, also run random event
            QString eventMessage = engine.runRandomEvent(stats, activity);

            if (!eventMessage.isEmpty())
            {
                QMessageBox::information(this, "Event", eventMessage);
            }

            // Check survival
            if (!stats.isAlive()) {
                finishSimulation("Your stats dropped too low. You lost the simulation.");
                ui->currentActivityLabel->setText(
                    "Failed during: " + QString::fromStdString(activity.name));
                return;
            }

            // Move to next activity
            currentIndex++;

            if (currentIndex >= static_cast<int>(schedule.activities.size())) {
                ui->currentActivityLabel->setText("Schedule complete.");
                finishSimulation("You survived the full schedule. You win!");
                return;
            }

            updateActivityDisplay();
        });

        return; // IMPORTANT: stop here so we don’t double-run
    }

    // 3️⃣ No decision → run random event
    QString eventMessage = engine.runRandomEvent(stats, activity);

    if (!eventMessage.isEmpty())
    {
        QMessageBox::information(this, "Event", eventMessage);
    }

    updateStatsDisplay();

    // Check survival
    if (!stats.isAlive()) {
        finishSimulation("Your stats dropped too low. You lost the simulation.");
        ui->currentActivityLabel->setText(
            "Failed during: " + QString::fromStdString(activity.name));
        return;
    }

    // Move forward
    currentIndex++;

    if (currentIndex >= static_cast<int>(schedule.activities.size())) {
        ui->currentActivityLabel->setText("Schedule complete.");
        finishSimulation("You survived the full schedule. You win!");
        return;
    }

    updateActivityDisplay();
}
