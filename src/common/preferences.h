#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "dialogs.h"
#include <stdio.h>

class Preferences
{
public:
    Preferences(QString prefPath = "prefs.json");
    void load();
    void save();

    QString themeName;
    QString language;
    QString rtcServer;
    int outputDeviceID;
    int inputDeviceID;
    bool showMixer;
    bool showLibrary;

    QList<QString> libraryDirList;
private:
    QString prefPath;
};

#endif // PREFERENCES_H
