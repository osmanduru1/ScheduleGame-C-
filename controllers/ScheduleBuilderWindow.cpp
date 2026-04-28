#include "ScheduleBuilderWindow.h"
#include "SimulationWindow.h"
#include "ui_schedule_builder.h"

#include <QMessageBox>
#include <algorithm>

static const QStringList DAY_NAMES = {
    "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
};

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

    connect(ui->scheduleTable, &QTableWidget::itemSelectionChanged,
        this, &ScheduleBuilderWindow::onTableSelectionChanged);

    connect(ui->dayComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &ScheduleBuilderWindow::onDayChanged);

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
    ui->scheduleTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->scheduleTable->setSelectionBehavior(QAbstractItemView::SelectItems);

    // Day dropdown
    for (const QString& day : DAY_NAMES)
        ui->dayComboBox->addItem(day);

    // Time dropdowns
    for (int h = 0; h < 24; h++)
    {
        QString time = QString("%1:00").arg(h,2,10,QChar('0'));
        ui->startTimeBox->addItem(time);
        ui->endTimeBox->addItem(time);
    }

    // Calendar table — 96 quarter-hour rows x 7 day columns
    ui->scheduleTable->setRowCount(96);
    ui->scheduleTable->setColumnCount(7);
    ui->scheduleTable->verticalHeader()->setDefaultSectionSize(12);

    QStringList timeLabels;
    for (int i = 0; i < 96; i++)
    {
        int hour   = i / 4;
        int minute = (i % 4) * 15;
        timeLabels << QString("%1:%2")
                      .arg(hour,   2, 10, QChar('0'))
                      .arg(minute, 2, 10, QChar('0'));
    }
    ui->scheduleTable->setVerticalHeaderLabels(timeLabels);
    ui->scheduleTable->setHorizontalHeaderLabels(DAY_NAMES);

    // Resize columns evenly
    for (int col = 0; col < 7; col++)
        ui->scheduleTable->horizontalHeader()->setSectionResizeMode(col, QHeaderView::Stretch);
}

void ScheduleBuilderWindow::onDayChanged(int dayIndex)
{
    currentDay = dayIndex;
    updateRemainingHours();
}

void ScheduleBuilderWindow::rebuildTableForDay(int day)
{
    for (int r = 0; r < 96; r++)
    {
        auto* existing = ui->scheduleTable->item(r, day);
        if (existing)
            delete ui->scheduleTable->takeItem(r, day);
    }

    for (const Activity& a : weekSchedules[day].activities)
    {
        int startRow = a.startHour * 4;
        int endRow   = a.endHour   * 4;

        for (int r = startRow; r < endRow; r++)
        {
            QString text =
                QString::fromStdString(a.name) + " | " + activityTypeToString(a.type);
            ui->scheduleTable->setItem(r, day, new QTableWidgetItem(text));
        }
    }
}

void ScheduleBuilderWindow::onTableSelectionChanged()
{
    auto selected = ui->scheduleTable->selectedRanges();
    if (selected.isEmpty()) return;

    int topRow    = selected.first().topRow();
    int bottomRow = selected.first().bottomRow();

    int startHour = topRow / 4;
    int endHour   = (bottomRow / 4) + 1;
    if (endHour > 24) endHour = 24;

    ui->startTimeBox->setCurrentIndex(startHour);
    ui->endTimeBox->setCurrentIndex(endHour);

    // Switch day selector to the clicked column
    int col = selected.first().leftColumn();
    if (col >= 0 && col < 7)
    {
        ui->dayComboBox->blockSignals(true);
        ui->dayComboBox->setCurrentIndex(col);
        ui->dayComboBox->blockSignals(false);
        currentDay = col;
        updateRemainingHours();
    }
}

void ScheduleBuilderWindow::onDeleteActivityClicked()
{
    auto selected = ui->scheduleTable->selectedRanges();
    if (selected.isEmpty()) return;

    int col = selected.first().leftColumn();
    int row = selected.first().topRow();

    if (col < 0 || col >= 7) return;

    int clickedHour = row / 4;

    auto& daySchedule = weekSchedules[col];
    auto it = std::find_if(
        daySchedule.activities.begin(),
        daySchedule.activities.end(),
        [clickedHour](const Activity& a) {
            return clickedHour >= a.startHour && clickedHour < a.endHour;
        });

    if (it == daySchedule.activities.end()) return;

    daySchedule.activities.erase(it);
    rebuildTableForDay(col);
    updateRemainingHours();
}

ActivityType ScheduleBuilderWindow::stringToActivityType(const QString& typeText) const
{
    if (typeText == "Class")    return ActivityType::Class;
    if (typeText == "Study")    return ActivityType::Study;
    if (typeText == "Work")     return ActivityType::Work;
    if (typeText == "Meal")     return ActivityType::Meal;
    if (typeText == "Sleep")    return ActivityType::Sleep;
    if (typeText == "Exercise") return ActivityType::Exercise;
    return ActivityType::Break;
}

QString ScheduleBuilderWindow::activityTypeToString(ActivityType type) const
{
    switch(type)
    {
        case ActivityType::Class:    return "Class";
        case ActivityType::Study:    return "Study";
        case ActivityType::Work:     return "Work";
        case ActivityType::Meal:     return "Meal";
        case ActivityType::Sleep:    return "Sleep";
        case ActivityType::Exercise: return "Exercise";
        case ActivityType::Break:    return "Break";
    }
    return "Unknown";
}

void ScheduleBuilderWindow::onAddActivityClicked()
{
    QString name     = ui->activityNameEdit->text().trimmed();
    QString typeText = ui->activityTypeComboBox->currentText();

    int start = ui->startTimeBox->currentIndex();
    int end   = ui->endTimeBox->currentIndex();

    if (end <= start)
    {
        QMessageBox::warning(this, "Invalid Time",
                             "End hour must be after start hour.");
        return;
    }

    if (name.isEmpty())
    {
        QMessageBox::warning(this, "Missing Activity Name",
                             "Please enter an activity name.");
        return;
    }

    ActivityType type = stringToActivityType(typeText);
    Activity activity(name.toStdString(), type, start, end);

    auto& daySchedule = weekSchedules[currentDay];
    daySchedule.addActivity(activity);

    std::sort(daySchedule.activities.begin(),
              daySchedule.activities.end(),
              [](const Activity& a, const Activity& b) {
                  return a.startHour < b.startHour;
              });

    rebuildTableForDay(currentDay);
    updateRemainingHours();

    ui->activityNameEdit->clear();
    ui->activityNameEdit->setFocus();
}

void ScheduleBuilderWindow::onStartSimulationClicked()
{
    // Collect all activities across all days for the simulation
    Schedule combined;
    for (const auto& sched : weekSchedules)
        for (const Activity& a : sched.activities)
            combined.addActivity(a);

    if (combined.activities.empty())
    {
        QMessageBox::warning(this, "Empty Schedule",
                             "Please add at least one activity.");
        return;
    }

    if (simulationWindow)
        delete simulationWindow;

    simulationWindow = new SimulationWindow(combined, this);
    simulationWindow->show();
}

void ScheduleBuilderWindow::updateRemainingHours()
{
    int total = 0;
    for (const Activity& a : weekSchedules[currentDay].activities)
        total += (a.endHour - a.startHour);

    int remaining = 24 - total;

    QString dayName = DAY_NAMES[currentDay];
    ui->remainingHoursLabel->setText(
        dayName + " – Remaining Hours: " + QString::number(remaining));
}
