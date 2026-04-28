#pragma once

#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>
#include "DecisionEvent.h"

class DecisionCard : public QWidget
{
    Q_OBJECT

public:
    explicit DecisionCard(const DecisionEvent& decisionEvent, QWidget *parent = nullptr);

signals:
    void decisionMade(const DecisionOption& option);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPoint startPos;
    int originalX;
    DecisionEvent cardEvent;

    QLabel* label;
};