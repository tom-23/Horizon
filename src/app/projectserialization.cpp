#include "projectserialization.h"

ProjectSerialization::ProjectSerialization()
{

}


std::string ProjectSerialization::serialize(AudioManager &audioMan) {

    QJsonDocument jsonDocument;
    QJsonObject root;


    root.insert("Application", "Horizon");

    QJsonArray trackArray;

    for(int i = 0; i < audioMan.getTrackListCount() - 1; i++) {

        QJsonObject trackObject;
        Track *track = audioMan.getTrackByIndex(i);
        trackObject.insert("type", "track");
        trackObject.insert("index", track->getIndex());
        trackObject.insert("mute", track->getMute());

        QJsonArray audioRegionArray;

        for(int j = 0; j < track->getAudioRegionListCount(); j++) {

            QJsonObject audioRegionObject;
            AudioRegion *audioRegion = track->getAudioRegionByIndex(j);
            audioRegionObject.insert("type", "audioRegion");
            audioRegionObject.insert("gridLocation", audioRegion->getGridLocation());
            audioRegionObject.insert("filePath", QString::fromStdString(audioRegion->getLoadedFileName()));

            audioRegionArray.append(audioRegionObject);
        }

        trackObject.insert("audioRegions", audioRegionArray);
        trackArray.append(trackObject);
    }

    root.insert("tracks", trackArray);
    jsonDocument.setObject(root);

    return jsonDocument.toJson().toStdString();

}
