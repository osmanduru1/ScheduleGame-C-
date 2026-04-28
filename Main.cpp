/*
 * main.cpp
 *
 * Entry point of the application.
 *
 * Responsibilities:
 *  - Initialize the Qt application
 *  - Create the main window (ScheduleBuilderWindow)
 *  - Start the event loop
 *
 * Flow:
 *  1. QApplication is initialized
 *  2. Main window is created and displayed
 *  3. Event loop runs until the application exits
 */

#include <QApplication>
#include "controllers/ScheduleBuilderWindow.h"

int main(int argc, char *argv[]) {
    /*
     * Create Qt application instance.
     * Required for handling UI, events, and system integration.
     */
    QApplication app(argc, argv);

    /*
     * Create and display the main window.
     * This is the entry point for user interaction.
     */
    ScheduleBuilderWindow window;
    window.show();

    /*
     * Start the Qt event loop.
     * The program runs until the user closes the application.
     */
    return app.exec();
}