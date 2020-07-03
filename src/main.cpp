#include "gui/mainwindow.h"
#include "common/debug.h"
#include "common/util.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    QFontDatabase::addApplicationFont(":/fonts/Rubik/Rubik-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Rubik/Rubik-Medium.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Rubik/Rubik-MediumItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Rubik/Rubik-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Rubik/Rubik-Italic.ttf");

    debug::setDebugLevel(3);
    debug::out(3, "Horizon Digital Audio Workstation");
    debug::out(3, "Loading MainWindow...");
    MainWindow w;
    w.show();
    return a.exec();
}
