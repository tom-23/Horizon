#ifndef PROJECTSERIALIZATION_H
#define PROJECTSERIALIZATION_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <chrono>
#include "app/audiomanager.h"

class ProjectSerialization : public QObject
{
    Q_OBJECT
public:
    ProjectSerialization();
    std::string serialize(AudioManager &audioMan, bool epoch);
    void deSerialize(std::string json, AudioManager &audioMan);

    bool compaire(std::string a, std::string b);

    bool copyToTemp = false;

    QList<QList<QString>> tempFileList;

    QString sessionID = "";

private:

    QByteArray fileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm);
};

#endif // PROJECTSERIALIZATION_H
