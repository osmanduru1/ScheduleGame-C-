#include <QApplication>
#include "controllers/ScheduleBuilderWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ScheduleBuilderWindow window;
    window.show();
    return app.exec();
}
