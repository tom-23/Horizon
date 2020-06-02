#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H
#include <QString>
#include <QWidget>
#include <QJsonDocument>
#include <QFile>


class ThemeManager
{
public:
    ThemeManager(QWidget *_parent, QString _themeFileName);
    QString generateQSS(QString _themeFileName);
    void setThemeFile(QString _themeFileName);
    void updateTheme();
    QColor getColor(QString key);
private:
    QString themeFileName;
    QWidget *parent;
    QJsonObject *colorTheme;
};

#endif // THEMEMANAGER_H
