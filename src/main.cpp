#include "gui/mainwindow.h"
#include "common/debug.h"
#include "common/util.h"
#include <QApplication>
#include <QSplashScreen>


int main(int argc, char *argv[])
{
    //QtWebView::initialize();

    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);

    debug::setDebugLevel(3);
    debug::out(3, "Horizon Digital Audio Workstation");

    QPixmap pixmap(":/splash/Untitled-2-01.svg");
    QSplashScreen splash(pixmap);
    splash.setMask(pixmap.mask());
    splash.show();

    debug::out(3, "Loading fonts...");
    //Q_INIT_RESOURCE(resources);

    qDebug() << QFontDatabase::addApplicationFont(":/fonts/fonts/Rublik/Rubik-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fonts/Rublik/Rubik-Medium.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fonts/Rublik/Rubik-MediumItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fonts/Rublik/Rubik-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fonts/Rublik/Rubik-Italic.ttf");


    debug::out(3, "Loading MainWindow...");
    MainWindow w;
    w.show();
    return a.exec();
}
