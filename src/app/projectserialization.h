#ifndef PROJECTSERIALIZATION_H
#define PROJECTSERIALIZATION_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include "app/audiomanager.h"

class ProjectSerialization : public QObject
{
    Q_OBJECT
public:
    ProjectSerialization();
    std::string serialize(AudioManager &audioMan);
};

#endif // PROJECTSERIALIZATION_H
