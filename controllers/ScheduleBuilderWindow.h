#pragma once

#include <QMainWindow>
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

private:
    Ui::ScheduleBuilderWindow *ui;
    Schedule currentSchedule;
    SimulationWindow *simulationWindow;
    void updateRemainingHours();   // <-- ADD THIS LINE


    void setupUiData();
    ActivityType stringToActivityType(const QString& typeText) const;
    QString activityTypeToString(ActivityType type) const;
};