#include "projectserialization.h"

ProjectSerialization::ProjectSerialization()
{
    tempFileList = {};
}

// TODO: refactor serialize and deSerialize to be less complicated and more modular.
std::string ProjectSerialization::serialize(AudioManager &audioMan, bool epoch) {

    QJsonDocument jsonDocument;
    QJsonObject root;
    // here is where we take note of *most* of the currrent values...
    debug::out(3, "Starting deserialisation...");
    root.insert("Application", "Horizon");
    root.insert("tempo", audioMan.getBPM());

    // ...and the point is?
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch()
    );
    if (epoch == true) {
        root.insert("TS_EPOCH", QString::fromStdString(std::to_string(ms.count())));
    }


    QJsonArray trackArray;

    for(int i = 0; i < audioMan.getTrackListCount(); i++) {

        QJsonObject trackObject;
        Track *track = audioMan.trackList->at(i);
        trackObject.insert("type", "track");
        trackObject.insert("uuid", track->getUUID());
        trackObject.insert("index", track->getIndex());
        trackObject.insert("mute", track->getMute());
        trackObject.insert("gain", QString::number(track->getGain()));
        trackObject.insert("pan", QString::number(track->getPan()));
        trackObject.insert("color", track->getColor().name(QColor::HexRgb));

        QJsonArray audioRegionArray;

        for(int j = 0; j < track->getAudioRegionListCount(); j++) {

            QJsonObject audioRegionObject;
            AudioRegion *audioRegion = track->getAudioRegionByIndex(j);
            audioRegionObject.insert("type", "audioRegion");
            audioRegionObject.insert("uuid", audioRegion->getUUID());
            audioRegionObject.insert("gridLocation", QString::number(audioRegion->getGridLocation()));
            if (copyToTemp == true) { // this is set to true if we are transfering a project file. we just save audio
                //file to a temp directory.
                // on mac os the directory structure would look something like this:
                // /Users/<username>/Music/Horzion/<sessionID>/<audioFileHash>/<audioFileName>.wav
                // we take the hash of the audio file to avoid conflics. This could be implemeted in a faster way
                // but on modern day computers, its fast enough.
                // example of a file location (on macOS):
                // /Users/tombutcher/Music/Horzion/8f00b3cc-c489-45ee-8092-40d72d49639d/B39356855B818C63921EF3D0D78D06947ADFB435/audioFile.wav

                QByteArray byteArray = fileChecksum(audioRegion->getLoadedFileName(), QCryptographicHash::Sha1);
                QString checkSUM = QString::fromUtf8(byteArray.toHex());
                QString tempFilePath = "/" + sessionID + "/" + checkSUM + "/" + QFileInfo(audioRegion->getLoadedFileName()).fileName();

                bool exists = false; // if we've already coppied that specifc file, don't copy it again.
                for (int i = 0; i < int(tempFileList.size()); i++) {
                    if (tempFileList.at(i).at(1) == checkSUM) {
                        exists = true;
                    }
                }
                if (!exists) {
                    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Horizon";

                    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Horizon/" + sessionID + "/" + checkSUM;
                    QDir dir(dirPath);
                    dir.mkpath(dirPath); // make the path I was just taking about


                    if (QFile::copy(audioRegion->getLoadedFileName(), tempDir + tempFilePath)) {
                        debug::out(3, "Coppied source file to temp session directory");
                    } else {
                        debug::out(1, "Could not copy source file to temp session directory");
                    }

                    QList<QString> list;
                    list.append(tempFilePath);
                    list.append(checkSUM);
                    tempFileList.push_back(list);
                }
                audioRegionObject.insert("filePath", tempFilePath); // add the new path to the JSON object.
                audioRegionObject.insert("tempLocation", true);
            } else {
                audioRegionObject.insert("filePath", audioRegion->getLoadedFileName());
                audioRegionObject.insert("tempLocation", false);
            }

            audioRegionArray.append(audioRegionObject); // append the audio region to the track audio region array.
        }

        trackObject.insert("audioRegions", audioRegionArray); // append the array to the track obj.
        trackArray.append(trackObject);
    }

    root.insert("tracks", trackArray); // append the track to the tracklist array.
    jsonDocument.setObject(root);  // set the root object

    return jsonDocument.toJson().toStdString(); // make the json!

}

void ProjectSerialization::deSerialize(std::string json, AudioManager &audioMan) {

    // for the most part, most of this is the exact same as the serialization code from above
    // but with a few changes...
    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString::fromStdString(json).toUtf8());
    QJsonObject root = jsonDocument.object();

    audioMan.setBPM(root.value("tempo").toDouble());

    for (int i = 0; i < root.value("tracks").toArray().size(); i++) {

        QJsonObject trackJSON = root.value("tracks").toArray().at(i).toObject();
        if (trackJSON.value("type") == "track") {
            debug::out(3, "Adding track");
            QString trackUuid;
            if (trackJSON.value("uuid").toString() == "") {
                trackUuid = QUuid::createUuid().toString();
            } else {
                trackUuid = trackJSON.value("uuid").toString();
            }
            Track *track = audioMan.addTrack(trackUuid);

            for (int ar = 0; ar < trackJSON.value("audioRegions").toArray().size(); ar++) {
                QJsonObject audioRegionJSON = trackJSON.value("audioRegions").toArray().at(ar).toObject();

                if (audioRegionJSON.value("type").toString() == "audioRegion") {
                    debug::out(3, "Adding audio region");
                    QString regionUuid;
                    if (audioRegionJSON.value("uuid").toString() == "") {
                        regionUuid = QUuid::createUuid().toString();
                    } else {
                        regionUuid = audioRegionJSON.value("uuid").toString();
                    }
                    AudioRegion *audioRegion = track->addAudioRegion(regionUuid);
                    audioRegion->setGridLocation(std::stod(audioRegionJSON.value("gridLocation").toString().toStdString()));
                    if (audioRegionJSON.value("tempLocation").toBool()) { // if this file is being sent from the web...
                        QString tempDir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Horizon";
                        audioRegion->preLoadedFile = tempDir + audioRegionJSON.value("filePath").toString();
                    } else {
                        audioRegion->preLoadedFile = audioRegionJSON.value("filePath").toString();
                    }

                }
            }

            track->setGain(std::stof(trackJSON.value("gain").toString().toStdString()));
            track->setPan(std::stof(trackJSON.value("pan").toString().toStdString()));
            track->setMute(trackJSON.value("mute").toBool());
            QColor color;
            color.setNamedColor(trackJSON.value("color").toString());
            track->setColor(color);
        }
    }
}

bool ProjectSerialization::compaire(std::string a, std::string b) {
    QJsonDocument aJSON = QJsonDocument::fromJson(QString::fromStdString(a).toUtf8());
    QJsonDocument bJSON = QJsonDocument::fromJson(QString::fromStdString(b).toUtf8());

    QString aCompact = aJSON.toJson(QJsonDocument::Compact);
    QString bCompact = bJSON.toJson(QJsonDocument::Compact);

    return aCompact == bCompact;
}

QByteArray ProjectSerialization::fileChecksum(const QString &fileName,
                        QCryptographicHash::Algorithm hashAlgorithm)
{
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(hashAlgorithm);
        if (hash.addData(&f)) {
            return hash.result();
        }
    }
    return QByteArray();
}
