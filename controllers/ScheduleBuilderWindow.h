#pragma once

#include <QMainWindow>
#include <array>
#include "../models/Schedule.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ScheduleBuilderWindow;
}
QT_END_NAMESPACE

class SimulationWindow;

class ScheduleBuilderWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScheduleBuilderWindow(QWidget *parent = nullptr);
    ~ScheduleBuilderWindow();

private slots:
    void onAddActivityClicked();
    void onStartSimulationClicked();
    void onDeleteActivityClicked();
    void onDayChanged(int dayIndex);

private:
    Ui::ScheduleBuilderWindow *ui;
    std::array<Schedule, 7> weekSchedules;  // One schedule per day
    int currentDay = 0;                      // 0=Mon ... 6=Sun
    SimulationWindow *simulationWindow;
    void updateRemainingHours();
    void onTableSelectionChanged();
    void rebuildTableForDay(int day);

    void setupUiData();
    ActivityType stringToActivityType(const QString& typeText) const;
    QString activityTypeToString(ActivityType type) const;
};