#include "ScheduleBuilderWindow.h"
#include "SimulationWindow.h"
#include "ui_schedule_builder.h"

#include <QMessageBox>
#include <algorithm>

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

    connect(ui->deleteActivityButton, &QPushButton::clicked,
            this, &ScheduleBuilderWindow::onDeleteActivityClicked);

    updateRemainingHours();
}

ScheduleBuilderWindow::~ScheduleBuilderWindow()
{
    delete simulationWindow;
    delete ui;
}

void ScheduleBuilderWindow::setupUiData()
{
    // Activity types
    ui->activityTypeComboBox->addItem("Class");
    ui->activityTypeComboBox->addItem("Study");
    ui->activityTypeComboBox->addItem("Work");
    ui->activityTypeComboBox->addItem("Meal");
    ui->activityTypeComboBox->addItem("Sleep");
    ui->activityTypeComboBox->addItem("Exercise");
    ui->activityTypeComboBox->addItem("Break");

    // Time dropdowns
    for (int h = 0; h < 24; h++)
    {
        QString time = QString("%1:00").arg(h,2,10,QChar('0'));

        ui->startTimeBox->addItem(time);
        ui->endTimeBox->addItem(time);
    }

    // Calendar table
    ui->scheduleTable->setRowCount(96);
    ui->scheduleTable->setColumnCount(1);

    QStringList timeLabels;

    for (int i = 0; i < 96; i++)
    {
        int hour = i / 4;
        int minute = (i % 4) * 15;

        QString label =
            QString("%1:%2")
            .arg(hour,2,10,QChar('0'))
            .arg(minute,2,10,QChar('0'));

        timeLabels << label;
    }

    ui->scheduleTable->setVerticalHeaderLabels(timeLabels);
    ui->scheduleTable->setHorizontalHeaderLabels({"Schedule"});
    ui->scheduleTable->horizontalHeader()->setStretchLastSection(true);
}

void ScheduleBuilderWindow::onDeleteActivityClicked()
{
    int row = ui->scheduleTable->currentRow();

    if (row < 0)
        return;

    ui->scheduleTable->clearContents();
    currentSchedule.activities.clear();

    updateRemainingHours();
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
    switch(type)
    {
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

    int start = ui->startTimeBox->currentIndex();
    int end   = ui->endTimeBox->currentIndex();

    if (end <= start)
    {
        QMessageBox::warning(this,"Invalid Time",
                             "End hour must be after start hour.");
        return;
    }

    if (name.isEmpty())
    {
        QMessageBox::warning(this,"Missing Activity Name",
                             "Please enter an activity name.");
        return;
    }

    ActivityType type = stringToActivityType(typeText);
    Activity activity(name.toStdString(), type, start, end);

    currentSchedule.addActivity(activity);

    std::sort(currentSchedule.activities.begin(),
              currentSchedule.activities.end(),
              [](const Activity& a,const Activity& b)
              {
                  return a.startHour < b.startHour;
              });

    ui->scheduleTable->clearContents();

    for (const Activity& a : currentSchedule.activities)
    {
        int startRow = a.startHour * 4;
        int endRow   = a.endHour * 4;

        for(int r=startRow;r<endRow;r++)
        {
            QString item =
                QString::fromStdString(a.name) +
                " | " +
                activityTypeToString(a.type);

            QTableWidgetItem* cell = new QTableWidgetItem(item);

            ui->scheduleTable->setItem(r,0,cell);
        }
    }

    updateRemainingHours();

    ui->activityNameEdit->clear();
    ui->activityNameEdit->setFocus();
}

void ScheduleBuilderWindow::onStartSimulationClicked()
{
    if (currentSchedule.activities.empty())
    {
        QMessageBox::warning(this,"Empty Schedule",
                             "Please add at least one activity.");
        return;
    }

    if (simulationWindow)
        delete simulationWindow;

    simulationWindow = new SimulationWindow(currentSchedule,this);
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