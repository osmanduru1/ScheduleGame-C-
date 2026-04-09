#include "SimulationWindow.h"
#include "ui_simulation.h"

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

    ui->resultLabel->setText("");
    updateStatsDisplay();
    updateActivityDisplay();

    connect(ui->nextActivityButton, &QPushButton::clicked,
            this, &SimulationWindow::onNextActivityClicked);
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

        QString text = "Current Activity: "
                     + QString::fromStdString(activity.name)
                     + " (" + QString::number(activity.duration) + " hour(s))";

        ui->currentActivityLabel->setText(text);
    } else {
        ui->currentActivityLabel->setText("No more activities.");
    }
}

void SimulationWindow::finishSimulation(const QString& message)
{
    ui->resultLabel->setText(message);
    ui->nextActivityButton->setEnabled(false);
}

void SimulationWindow::onNextActivityClicked()
{
    if (currentIndex >= static_cast<int>(schedule.activities.size())) {
        finishSimulation("You survived the full schedule. You win!");
        return;
    }

    const Activity& activity = schedule.activities[currentIndex];
    engine.runActivity(stats, activity);

    updateStatsDisplay();

    if (!stats.isAlive()) {
        finishSimulation("Your stats dropped too low. You lost the simulation.");
        ui->currentActivityLabel->setText(
            "Failed during: " + QString::fromStdString(activity.name)
        );
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