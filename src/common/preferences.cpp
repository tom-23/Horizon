#include "preferences.h"

Preferences::Preferences(QString _prefsPath)
{
    prefPath = _prefsPath;
}

void Preferences::load() {
    QFile prefs(prefPath);
    if (prefs.open(QFile::ReadOnly)) {

        QJsonDocument prefsJSON = QJsonDocument::fromJson(prefs.readAll());
        QJsonObject *prefsObject = new QJsonObject(prefsJSON.object());
        themeName = prefsObject->value("themeName").toString();
        language = prefsObject->value("lang").toString();


        showMixer = prefsObject->value("showMixerOnStartup").toBool();
        showLibrary = prefsObject->value("showLibraryOnStartup").toBool();

        QJsonArray libDirArray = prefsObject->value("libraryFolders").toArray();
        libraryDirList.clear();
        for (int i = 0; i < libDirArray.size(); i++)  {
            libraryDirList.append(libDirArray.at(i).toString());
        }

    } else {
        debug::out(1, "COULD NOT READ PREFS FILE.");
        std::exit(1);
    }
}

void Preferences::save() {
    QFile prefs(prefPath);
    if (prefs.open(QFile::WriteOnly)) {

        QJsonDocument prefsJSON;
        QJsonObject prefsObject;

        prefsObject.insert("themeName", themeName);
        prefsObject.insert("lang", language);
        prefsObject.insert("showMixerOnStartup", showMixer);
        prefsObject.insert("showLibraryOnStartup", showLibrary);

        QJsonArray libDirArray;

        for (int i = 0; i < libraryDirList.size(); i++) {
            libDirArray.append(libraryDirList.at(i));
        }

        prefsObject.insert("libraryFolders", libDirArray);

        prefsJSON.setObject(prefsObject);

        prefs.write(prefsJSON.toJson());

    } else {
        debug::out(1, "COULD NOT READ PREFS FILE.");
        std::exit(1);
    }
}
