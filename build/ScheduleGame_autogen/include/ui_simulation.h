/********************************************************************************
** Form generated from reading UI file 'simulation.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMULATION_H
#define UI_SIMULATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SimulationWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QLabel *currentActivityLabel;
    QFormLayout *statsLayout;
    QLabel *label;
    QProgressBar *healthBar;
    QLabel *label1;
    QProgressBar *energyBar;
    QLabel *label2;
    QProgressBar *attentionBar;
    QLabel *label3;
    QProgressBar *stressBar;
    QLabel *label4;
    QProgressBar *sleepBar;
    QPushButton *nextActivityButton;
    QLabel *resultLabel;

    void setupUi(QMainWindow *SimulationWindow)
    {
        if (SimulationWindow->objectName().isEmpty())
            SimulationWindow->setObjectName("SimulationWindow");
        SimulationWindow->resize(480, 540);
        centralwidget = new QWidget(SimulationWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        currentActivityLabel = new QLabel(centralwidget);
        currentActivityLabel->setObjectName("currentActivityLabel");
        currentActivityLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(currentActivityLabel);

        statsLayout = new QFormLayout();
        statsLayout->setObjectName("statsLayout");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        statsLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        healthBar = new QProgressBar(centralwidget);
        healthBar->setObjectName("healthBar");
        healthBar->setValue(70);

        statsLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, healthBar);

        label1 = new QLabel(centralwidget);
        label1->setObjectName("label1");

        statsLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label1);

        energyBar = new QProgressBar(centralwidget);
        energyBar->setObjectName("energyBar");
        energyBar->setValue(70);

        statsLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, energyBar);

        label2 = new QLabel(centralwidget);
        label2->setObjectName("label2");

        statsLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label2);

        attentionBar = new QProgressBar(centralwidget);
        attentionBar->setObjectName("attentionBar");
        attentionBar->setValue(70);

        statsLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, attentionBar);

        label3 = new QLabel(centralwidget);
        label3->setObjectName("label3");

        statsLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label3);

        stressBar = new QProgressBar(centralwidget);
        stressBar->setObjectName("stressBar");
        stressBar->setValue(30);

        statsLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, stressBar);

        label4 = new QLabel(centralwidget);
        label4->setObjectName("label4");

        statsLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, label4);

        sleepBar = new QProgressBar(centralwidget);
        sleepBar->setObjectName("sleepBar");
        sleepBar->setValue(70);

        statsLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, sleepBar);


        verticalLayout->addLayout(statsLayout);

        nextActivityButton = new QPushButton(centralwidget);
        nextActivityButton->setObjectName("nextActivityButton");

        verticalLayout->addWidget(nextActivityButton);

        resultLabel = new QLabel(centralwidget);
        resultLabel->setObjectName("resultLabel");
        resultLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(resultLabel);

        SimulationWindow->setCentralWidget(centralwidget);

        retranslateUi(SimulationWindow);

        QMetaObject::connectSlotsByName(SimulationWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SimulationWindow)
    {
        SimulationWindow->setWindowTitle(QCoreApplication::translate("SimulationWindow", "Simulation", nullptr));
        titleLabel->setText(QCoreApplication::translate("SimulationWindow", "Simulation Running", nullptr));
        currentActivityLabel->setText(QCoreApplication::translate("SimulationWindow", "Current Activity: \342\200\224", nullptr));
        label->setText(QCoreApplication::translate("SimulationWindow", "Health:", nullptr));
        label1->setText(QCoreApplication::translate("SimulationWindow", "Energy:", nullptr));
        label2->setText(QCoreApplication::translate("SimulationWindow", "Attention:", nullptr));
        label3->setText(QCoreApplication::translate("SimulationWindow", "Stress:", nullptr));
        label4->setText(QCoreApplication::translate("SimulationWindow", "Sleep:", nullptr));
        nextActivityButton->setText(QCoreApplication::translate("SimulationWindow", "Next Activity", nullptr));
        resultLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SimulationWindow: public Ui_SimulationWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMULATION_H
