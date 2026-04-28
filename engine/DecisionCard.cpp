#include "DecisionCard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

DecisionCard::DecisionCard(const DecisionEvent& decisionEvent, QWidget *parent)
    : QWidget(parent), cardEvent(decisionEvent)
{
    setFixedSize(300, 230);
    setStyleSheet("background-color: white; border: 2px solid black; border-radius: 10px;");

    label = new QLabel(cardEvent.description, this);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: black; font-size: 14px; padding: 8px;");

    QLabel* leftHint = new QLabel("← " + cardEvent.leftOption.text, this);
    leftHint->setWordWrap(true);
    leftHint->setAlignment(Qt::AlignLeft);
    leftHint->setStyleSheet("color: #c0392b; font-size: 11px; padding: 4px;");

    QLabel* rightHint = new QLabel(cardEvent.rightOption.text + " →", this);
    rightHint->setWordWrap(true);
    rightHint->setAlignment(Qt::AlignRight);
    rightHint->setStyleSheet("color: #27ae60; font-size: 11px; padding: 4px;");

    QHBoxLayout* hintLayout = new QHBoxLayout();
    hintLayout->addWidget(leftHint);
    hintLayout->addWidget(rightHint);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addLayout(hintLayout);
    setLayout(layout);
}

void DecisionCard::mousePressEvent(QMouseEvent *event)
{
    startPos = event->pos();
    originalX = x();
}

void DecisionCard::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->pos().x() - startPos.x();

    move(x() + dx, y());
}

void DecisionCard::mouseReleaseEvent(QMouseEvent *event)
{
    int threshold = 100;
    int offset = x() - originalX;

    if (offset > threshold)
    {
        emit decisionMade(cardEvent.rightOption);
    }
    else if (offset < -threshold)
    {
        emit decisionMade(cardEvent.leftOption);
    }
    else
    {
        move(originalX, y()); // snap back to original position
    }
}