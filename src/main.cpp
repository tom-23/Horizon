#include "gui/mainwindow.h"
#include "common/debug.h"
#include "common/util.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    QFontDatabase::addApplicationFont(":/fonts/TitilliumWeb.ttf");
    QFontDatabase::addApplicationFont(":/fonts/TitilliumWeb-SemiBold.ttf");


    debug::setDebugLevel(3);
    debug::out(3, "Horizon Digital Audio Workstation");
    debug::out(3, "Loading MainWindow...");
    MainWindow w;
    w.show();
    return a.exec();
}
