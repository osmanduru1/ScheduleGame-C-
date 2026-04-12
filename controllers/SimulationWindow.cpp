#include "SimulationWindow.h"
#include "ui_simulation.h"

#include <cstdlib>
#include <ctime>
#include <QMessageBox>

SimulationWindow::SimulationWindow(const Schedule& schedule, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::SimulationWindow),
      schedule(schedule),
      currentIndex(0)
{
    ui->setupUi(this);

    ui->healthBar->setRange(0, 100);
    ui->energyBar->setRange(0, 100);
    ui->attentionBar->setRange(0, 100);
    ui->stressBar->setRange(0, 100);
    ui->sleepBar->setRange(0, 100);

    srand(time(nullptr));

    ui->resultLabel->setText("");

    updateStatsDisplay();
    updateActivityDisplay();

    connect(ui->nextActivityButton, &QPushButton::clicked,
            this, &SimulationWindow::onNextActivityClicked);

    connect(ui->restartButton, &QPushButton::clicked,
        this, &SimulationWindow::onRestartClicked);
}

SimulationWindow::~SimulationWindow()
{
    delete ui;
}

void SimulationWindow::updateStatsDisplay()
{
    ui->healthBar->setValue(stats.health);
    ui->energyBar->setValue(stats.energy);
    ui->attentionBar->setValue(stats.attention);
    ui->stressBar->setValue(stats.stress);
    ui->sleepBar->setValue(stats.sleep);
}

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

void SimulationWindow::onRestartClicked()
{
    this->close();
}

void SimulationWindow::finishSimulation(const QString& message)
{
    ui->nextActivityButton->setEnabled(false);

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

void SimulationWindow::onNextActivityClicked()
{
    if (currentIndex >= static_cast<int>(schedule.activities.size())) {
        finishSimulation("You survived the full schedule. You win!");
        return;
    }

    const Activity& activity = schedule.activities[currentIndex];

    engine.runActivity(stats, activity);

    QString eventMessage = engine.runRandomEvent(stats);

    if (!eventMessage.isEmpty())
    {
        QMessageBox::information(this, "Random Event", eventMessage);
    }

    updateStatsDisplay();

    if (!stats.isAlive()) {

        finishSimulation("Your stats dropped too low. You lost the simulation.");

        ui->currentActivityLabel->setText(
            "Failed during: " + QString::fromStdString(activity.name));

        return;
    }

    currentIndex++;

    if (currentIndex >= static_cast<int>(schedule.activities.size())) {

        ui->currentActivityLabel->setText("Schedule complete.");
        finishSimulation("You survived the full schedule. You win!");

        return;
    }

    updateActivityDisplay();
}