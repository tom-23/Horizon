#include "gui/mainwindow.h"
#include "common/debug.h"
#include "common/util.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    QFontDatabase::addApplicationFont(":/fonts/fonts/Raleway/Raleway-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fonts/Raleway/Raleway-Medium.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fonts/Raleway/Raleway-SemiBold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fonts/Raleway/Raleway-Bold.ttf");

    QFontDatabase::addApplicationFont(":/fonts/fonts/Raleway/Raleway-Italic.ttf");

    debug::setDebugLevel(3);
    debug::out(3, "Horizon Digital Audio Workstation");
    debug::out(3, "Loading MainWindow...");
    MainWindow w;
    w.show();
    return a.exec();
}
