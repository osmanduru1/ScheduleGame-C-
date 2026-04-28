#pragma once

#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>
#include "DecisionEvent.h"

/*
 * DecisionCard
 *
 * A custom interactive UI component representing a decision event.
 *
 * Purpose:
 *  - Displays a scenario (description + options)
 *  - Allows the user to make a choice via swipe gesture
 *
 * Interaction Model:
 *  - Click + drag horizontally
 *  - Drag right → select right option
 *  - Drag left  → select left option
 *  - Small movement → card resets to original position
 *
 * This component emits a signal when a decision is made,
 * which is handled by the SimulationWindow.
 */
class DecisionCard : public QWidget
{
    Q_OBJECT

public:

    /*
     * Constructor
     *
     * Initializes the card with a decision event,
     * including description and available options.
     */
    explicit DecisionCard(const DecisionEvent& decisionEvent, QWidget *parent = nullptr);

signals:

    /*
     * Emitted when the user makes a decision by swiping.
     *
     * The selected option (left or right) is passed to the listener.
     */
    void decisionMade(const DecisionOption& option);

protected:

    /*
     * Triggered when mouse is pressed on the card.
     *
     * Stores initial position for drag tracking.
     */
    void mousePressEvent(QMouseEvent *event) override;

    /*
     * Triggered while the mouse is dragged.
     *
     * Moves the card horizontally based on drag distance.
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /*
     * Triggered when mouse is released.
     *
     * Determines whether a decision is made or
     * if the card should return to its original position.
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

private:

    /*
     * Initial mouse position when drag starts.
     */
    QPoint startPos;

    /*
     * Original X position of the card before dragging.
     */
    int originalX;

    /*
     * Decision event data (description + options).
     */
    DecisionEvent cardEvent;

    /*
     * Main label displaying the decision description.
     */
    QLabel* label;
};