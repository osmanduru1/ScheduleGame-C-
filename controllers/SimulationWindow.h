#pragma once
#include <QWidget>
#include <QLabel>
#include <QMainWindow>
#include "../models/Schedule.h"
#include "../models/Stats.h"
#include "../engine/SimulationEngine.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SimulationWindow;
}
QT_END_NAMESPACE

class SimulationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimulationWindow(const Schedule& schedule, QWidget *parent = nullptr);
    ~SimulationWindow();

private slots:
    void onNextActivityClicked();
    void onRestartClicked();

private:
    Ui::SimulationWindow *ui;

    Schedule schedule;
    Stats stats;
    SimulationEngine engine;
    int currentIndex;

    void updateStatsDisplay();
    void updateActivityDisplay();
    void finishSimulation(const QString& message);
};