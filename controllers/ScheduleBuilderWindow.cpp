#include "ScheduleBuilderWindow.h"
#include "SimulationWindow.h"
#include "ui_schedule_builder.h"

#include <QMessageBox>

ScheduleBuilderWindow::ScheduleBuilderWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::ScheduleBuilderWindow),
      simulationWindow(nullptr)
{
    ui->setupUi(this);
    setupUiData();

    connect(ui->addActivityButton, &QPushButton::clicked,
            this, &ScheduleBuilderWindow::onAddActivityClicked);

    connect(ui->startSimulationButton, &QPushButton::clicked,
            this, &ScheduleBuilderWindow::onStartSimulationClicked);

    updateRemainingHours();
}

ScheduleBuilderWindow::~ScheduleBuilderWindow()
{
    delete simulationWindow;
    delete ui;
}

void ScheduleBuilderWindow::setupUiData()
{
    ui->activityTypeComboBox->addItem("Class");
    ui->activityTypeComboBox->addItem("Study");
    ui->activityTypeComboBox->addItem("Work");
    ui->activityTypeComboBox->addItem("Meal");
    ui->activityTypeComboBox->addItem("Sleep");
    ui->activityTypeComboBox->addItem("Exercise");
    ui->activityTypeComboBox->addItem("Break");


}

ActivityType ScheduleBuilderWindow::stringToActivityType(const QString& typeText) const
{
    if (typeText == "Class") return ActivityType::Class;
    if (typeText == "Study") return ActivityType::Study;
    if (typeText == "Work") return ActivityType::Work;
    if (typeText == "Meal") return ActivityType::Meal;
    if (typeText == "Sleep") return ActivityType::Sleep;
    if (typeText == "Exercise") return ActivityType::Exercise;

    return ActivityType::Break;
}

QString ScheduleBuilderWindow::activityTypeToString(ActivityType type) const
{
    switch (type) {
        case ActivityType::Class: return "Class";
        case ActivityType::Study: return "Study";
        case ActivityType::Work: return "Work";
        case ActivityType::Meal: return "Meal";
        case ActivityType::Sleep: return "Sleep";
        case ActivityType::Exercise: return "Exercise";
        case ActivityType::Break: return "Break";
    }

    return "Unknown";
}

void ScheduleBuilderWindow::onAddActivityClicked()
{
    QString name = ui->activityNameEdit->text().trimmed();
    QString typeText = ui->activityTypeComboBox->currentText();
    int start = ui->startHourSpinBox->value();
    int end = ui->endHourSpinBox->value();

    if (end <= start) {
        QMessageBox::warning(this, "Invalid Time",
                            "End hour must be after start hour.");
        return;
    }    

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Missing Activity Name",
                             "Please enter an activity name.");
        return;
    }

    int total = 0;
    for (const Activity& a : currentSchedule.activities)
    total += (a.endHour - a.startHour);

    int duration = end - start;

    if (total + duration > 24) {
        QMessageBox::warning(this, "Invalid Schedule",
                            "Total schedule time cannot exceed 24 hours.");
        return;
    }

    ActivityType type = stringToActivityType(typeText);
    Activity activity(name.toStdString(), type, start, end);

    currentSchedule.addActivity(activity);

    QString displayText =
        name + " | " + typeText +
        " | " + QString::number(start) + ":00 - " + QString::number(end) + ":00" + " hour(s)";

    ui->activityListWidget->addItem(displayText);

    updateRemainingHours();

    ui->activityNameEdit->clear();
    ui->activityNameEdit->setFocus();
}

void ScheduleBuilderWindow::onStartSimulationClicked()
{
    if (currentSchedule.activities.empty()) {
        QMessageBox::warning(this, "Empty Schedule",
                             "Please add at least one activity before starting the simulation.");
        return;
    }

    if (simulationWindow != nullptr) {
        delete simulationWindow;
        simulationWindow = nullptr;
    }

    simulationWindow = new SimulationWindow(currentSchedule, this);
    simulationWindow->show();
}

void ScheduleBuilderWindow::updateRemainingHours()
{
    int total = 0;

    for (const Activity& a : currentSchedule.activities)
    total += (a.endHour - a.startHour);

    int remaining = 24 - total;

    ui->remainingHoursLabel->setText(
        "Remaining Hours: " + QString::number(remaining));
}