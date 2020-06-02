#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    QFontDatabase::addApplicationFont(":/fonts/TitilliumWeb.ttf");
    QFontDatabase::addApplicationFont(":/fonts/TitilliumWeb-SemiBold.ttf");
    QFontDatabase db;
      for(int i=0; i<db.families().size(); i++)
      {
        qDebug() << db.families().at(i);
      }
    MainWindow w;
    w.show();
    return a.exec();
}
