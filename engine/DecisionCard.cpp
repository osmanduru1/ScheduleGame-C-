/*
 * DecisionCard.cpp
 *
 * This file implements a swipeable decision card UI component.
 *
 * Purpose:
 *  - Display a decision event to the user
 *  - Allow interaction via mouse drag (left/right swipe)
 *  - Emit a signal when a decision is made
 *
 * Interaction Model:
 *  - User clicks and drags the card horizontally
 *  - Drag right → selects right option
 *  - Drag left  → selects left option
 *  - If movement is too small, card snaps back to original position
 */

#include "DecisionCard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

/*
 * Constructor
 *
 * Initializes the visual layout of the decision card:
 *  - Main description text
 *  - Left and right option hints
 *  - Styling and layout structure
 */
DecisionCard::DecisionCard(const DecisionEvent& decisionEvent, QWidget *parent)
    : QWidget(parent), cardEvent(decisionEvent)
{
    // Set fixed card size and styling
    setFixedSize(300, 230);
    setStyleSheet("background-color: white; border: 2px solid black; border-radius: 10px;");

    /*
     * Main description label
     */
    label = new QLabel(cardEvent.description, this);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: black; font-size: 14px; padding: 8px;");

    /*
     * Left option hint (displayed with arrow)
     */
    QLabel* leftHint = new QLabel("← " + cardEvent.leftOption.text, this);
    leftHint->setWordWrap(true);
    leftHint->setAlignment(Qt::AlignLeft);
    leftHint->setStyleSheet("color: #c0392b; font-size: 11px; padding: 4px;");

    /*
     * Right option hint (displayed with arrow)
     */
    QLabel* rightHint = new QLabel(cardEvent.rightOption.text + " →", this);
    rightHint->setWordWrap(true);
    rightHint->setAlignment(Qt::AlignRight);
    rightHint->setStyleSheet("color: #27ae60; font-size: 11px; padding: 4px;");

    /*
     * Horizontal layout for option hints
     */
    QHBoxLayout* hintLayout = new QHBoxLayout();
    hintLayout->addWidget(leftHint);
    hintLayout->addWidget(rightHint);

    /*
     * Main vertical layout
     */
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addLayout(hintLayout);

    setLayout(layout);
}

/*
 * Triggered when mouse button is pressed on the card.
 *
 * Stores:
 *  - Initial mouse position
 *  - Original X position of the card
 */
void DecisionCard::mousePressEvent(QMouseEvent *event)
{
    startPos = event->pos();
    originalX = x();
}

/*
 * Triggered when mouse is dragged.
 *
 * Moves the card horizontally based on drag distance.
 */
void DecisionCard::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->pos().x() - startPos.x();

    move(x() + dx, y());
}

/*
 * Triggered when mouse button is released.
 *
 * Determines if a decision was made:
 *  - If dragged far enough right → right option
 *  - If dragged far enough left  → left option
 *  - Otherwise → snap back to original position
 */
void DecisionCard::mouseReleaseEvent(QMouseEvent *event)
{
    int threshold = 100;          // Minimum distance to trigger decision
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
        // Not enough movement → reset position
        move(originalX, y());
    }
}