/********************************************************************************
** Form generated from reading UI file 'schedule_builder.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCHEDULE_BUILDER_H
#define UI_SCHEDULE_BUILDER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScheduleBuilderWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QFormLayout *formLayout;
    QLabel *nameLabel;
    QLineEdit *activityNameEdit;
    QLabel *typeLabel;
    QComboBox *activityTypeComboBox;
    QLabel *startLabel;
    QSpinBox *startHourSpinBox;
    QLabel *endLabel;
    QSpinBox *endHourSpinBox;
    QPushButton *addActivityButton;
    QLabel *remainingHoursLabel;
    QLabel *listLabel;
    QListWidget *activityListWidget;
    QPushButton *startSimulationButton;

    void setupUi(QMainWindow *ScheduleBuilderWindow)
    {
        if (ScheduleBuilderWindow->objectName().isEmpty())
            ScheduleBuilderWindow->setObjectName("ScheduleBuilderWindow");
        ScheduleBuilderWindow->resize(480, 520);
        centralwidget = new QWidget(ScheduleBuilderWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        nameLabel = new QLabel(centralwidget);
        nameLabel->setObjectName("nameLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, nameLabel);

        activityNameEdit = new QLineEdit(centralwidget);
        activityNameEdit->setObjectName("activityNameEdit");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, activityNameEdit);

        typeLabel = new QLabel(centralwidget);
        typeLabel->setObjectName("typeLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, typeLabel);

        activityTypeComboBox = new QComboBox(centralwidget);
        activityTypeComboBox->setObjectName("activityTypeComboBox");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, activityTypeComboBox);

        startLabel = new QLabel(centralwidget);
        startLabel->setObjectName("startLabel");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, startLabel);

        startHourSpinBox = new QSpinBox(centralwidget);
        startHourSpinBox->setObjectName("startHourSpinBox");
        startHourSpinBox->setMinimum(0);
        startHourSpinBox->setMaximum(23);

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, startHourSpinBox);

        endLabel = new QLabel(centralwidget);
        endLabel->setObjectName("endLabel");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, endLabel);

        endHourSpinBox = new QSpinBox(centralwidget);
        endHourSpinBox->setObjectName("endHourSpinBox");
        endHourSpinBox->setMinimum(1);
        endHourSpinBox->setMaximum(24);

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, endHourSpinBox);


        verticalLayout->addLayout(formLayout);

        addActivityButton = new QPushButton(centralwidget);
        addActivityButton->setObjectName("addActivityButton");

        verticalLayout->addWidget(addActivityButton);

        remainingHoursLabel = new QLabel(centralwidget);
        remainingHoursLabel->setObjectName("remainingHoursLabel");
        remainingHoursLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(remainingHoursLabel);

        listLabel = new QLabel(centralwidget);
        listLabel->setObjectName("listLabel");

        verticalLayout->addWidget(listLabel);

        activityListWidget = new QListWidget(centralwidget);
        activityListWidget->setObjectName("activityListWidget");

        verticalLayout->addWidget(activityListWidget);

        startSimulationButton = new QPushButton(centralwidget);
        startSimulationButton->setObjectName("startSimulationButton");

        verticalLayout->addWidget(startSimulationButton);

        ScheduleBuilderWindow->setCentralWidget(centralwidget);

        retranslateUi(ScheduleBuilderWindow);

        QMetaObject::connectSlotsByName(ScheduleBuilderWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ScheduleBuilderWindow)
    {
        ScheduleBuilderWindow->setWindowTitle(QCoreApplication::translate("ScheduleBuilderWindow", "Schedule Builder", nullptr));
        titleLabel->setText(QCoreApplication::translate("ScheduleBuilderWindow", "Build Your Schedule", nullptr));
        nameLabel->setText(QCoreApplication::translate("ScheduleBuilderWindow", "Activity Name:", nullptr));
        typeLabel->setText(QCoreApplication::translate("ScheduleBuilderWindow", "Activity Type:", nullptr));
        startLabel->setText(QCoreApplication::translate("ScheduleBuilderWindow", "Start Hour:", nullptr));
        endLabel->setText(QCoreApplication::translate("ScheduleBuilderWindow", "End Hour:", nullptr));
        addActivityButton->setText(QCoreApplication::translate("ScheduleBuilderWindow", "Add Activity", nullptr));
        remainingHoursLabel->setText(QCoreApplication::translate("ScheduleBuilderWindow", "Remaining Hours: 24", nullptr));
        listLabel->setText(QCoreApplication::translate("ScheduleBuilderWindow", "Your Schedule:", nullptr));
        startSimulationButton->setText(QCoreApplication::translate("ScheduleBuilderWindow", "Start Simulation", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScheduleBuilderWindow: public Ui_ScheduleBuilderWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCHEDULE_BUILDER_H
