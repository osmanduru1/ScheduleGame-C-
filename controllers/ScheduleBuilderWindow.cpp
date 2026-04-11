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

    ui->durationSpinBox->setMinimum(1);
    ui->durationSpinBox->setMaximum(12);
    ui->durationSpinBox->setValue(1);
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
    int duration = ui->durationSpinBox->value();


    int total = 0;
    for (const Activity& a : currentSchedule.activities)
        total += a.duration;

    if (total + duration > 24) {
        QMessageBox::warning(this, "Invalid Schedule",
            "Total schedule time cannot exceed 24 hours.");
        return;
    }

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Missing Activity Name",
                             "Please enter an activity name.");
        return;
    }

    ActivityType type = stringToActivityType(typeText);
    Activity activity(name.toStdString(), type, duration);

    currentSchedule.addActivity(activity);

    QString displayText = name + " | " + typeText + " | " + QString::number(duration) + " hour(s)";
    ui->activityListWidget->addItem(displayText);

    ui->activityNameEdit->clear();
    ui->durationSpinBox->setValue(1);
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