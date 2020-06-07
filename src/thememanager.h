#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H
#include <QString>
#include <QWidget>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QDir>
#include <QTextStream>
#include "debug.h"


class ThemeManager
{
public:
    ThemeManager(QWidget *_parent, QString _themeFileName);
    QString generateQSS(QString _themeFileName);
    void setThemeFile(QString _themeFileName);
    void updateTheme();
    QColor getColor(QString key);

    QString colorizeSVG(QString filename);
private:
    QString themeFileName;
    QWidget *parent;
    QJsonObject *colorTheme;

};

#endif // THEMEMANAGER_H
