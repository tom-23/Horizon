#include "projectserialization.h"

ProjectSerialization::ProjectSerialization()
{
    tempFileList = {};
}


std::string ProjectSerialization::serialize(AudioManager &audioMan, bool epoch) {

    QJsonDocument jsonDocument;
    QJsonObject root;

    debug::out(3, "Starting deserialisation...");
    root.insert("Application", "Horizon");
    root.insert("tempo", audioMan.getBPM());

    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch()
    );
    if (epoch == true) {
        root.insert("TS_EPOCH", QString::fromStdString(std::to_string(ms.count())));
    }


    QJsonArray trackArray;

    for(int i = 0; i < audioMan.getTrackListCount(); i++) {

        QJsonObject trackObject;
        Track *track = audioMan.getTrackByIndex(i);
        trackObject.insert("type", "track");
        trackObject.insert("uuid", QString::fromStdString(track->getUUID()));
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
            audioRegionObject.insert("uuid", QString::fromStdString(audioRegion->getUUID()));
            audioRegionObject.insert("gridLocation", QString::number(audioRegion->getGridLocation()));
            if (copyToTemp == true) {

                QByteArray byteArray = fileChecksum(QString::fromStdString(audioRegion->getLoadedFileName()), QCryptographicHash::Sha1);
                QString checkSUM = QString::fromUtf8(byteArray.toHex());
                QString tempFilePath = "/" + sessionID + "/" + checkSUM + "/" + QFileInfo(QString::fromStdString(audioRegion->getLoadedFileName())).fileName();

                bool exists = false;
                for (int i = 0; i < int(tempFileList.size()); i++) {
                    if (tempFileList.at(i).at(1) == checkSUM) {
                        exists = true;
                    }
                }
                if (!exists) {
                    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Horizon";

                    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Horizon/" + sessionID + "/" + checkSUM;
                    QDir dir(dirPath);
                    dir.mkpath(dirPath);


                    if (QFile::copy(QString::fromStdString(audioRegion->getLoadedFileName()), tempDir + tempFilePath)) {
                        debug::out(3, "Coppied source file to temp session directory");
                    } else {
                        debug::out(1, "Could not copy source file to temp session directory");
                    }

                    QList<QString> list;
                    list.append(tempFilePath);
                    list.append(checkSUM);
                    tempFileList.push_back(list);
                }
                audioRegionObject.insert("filePath", tempFilePath);
                audioRegionObject.insert("tempLocation", true);
            } else {
                audioRegionObject.insert("filePath", QString::fromStdString(audioRegion->getLoadedFileName()));
                audioRegionObject.insert("tempLocation", false);
            }

            audioRegionArray.append(audioRegionObject);
        }

        trackObject.insert("audioRegions", audioRegionArray);
        trackArray.append(trackObject);
    }

    root.insert("tracks", trackArray);
    jsonDocument.setObject(root);

    return jsonDocument.toJson().toStdString();

}

void ProjectSerialization::deSerialize(std::string json, AudioManager &audioMan) {
    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString::fromStdString(json).toUtf8());
    QJsonObject root = jsonDocument.object();

    audioMan.setBPM(root.value("tempo").toDouble());

    for (int i = 0; i < root.value("tracks").toArray().size(); i++) {

        QJsonObject trackJSON = root.value("tracks").toArray().at(i).toObject();
        if (trackJSON.value("type") == "track") {
            debug::out(3, "Adding track");
            Track *track = audioMan.addTrack(trackJSON.value("uuid").toString().toStdString());

            for (int ar = 0; ar < trackJSON.value("audioRegions").toArray().size(); ar++) {
                QJsonObject audioRegionJSON = trackJSON.value("audioRegions").toArray().at(ar).toObject();

                if (audioRegionJSON.value("type").toString() == "audioRegion") {
                    debug::out(3, "Adding audio region");
                    AudioRegion *audioRegion = track->addAudioRegion(audioRegionJSON.value("uuid").toString().toStdString());
                    audioRegion->setGridLocation(std::stod(audioRegionJSON.value("gridLocation").toString().toStdString()));
                    qDebug() << QString::fromStdString(audioRegionJSON.value("filePath").toString().toStdString());
                    if (audioRegionJSON.value("tempLocation").toBool()) {
                        QString tempDir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Horizon";
                        audioRegion->preLoadedFile = (tempDir + audioRegionJSON.value("filePath").toString()).toStdString();
                    } else {
                        audioRegion->preLoadedFile = audioRegionJSON.value("filePath").toString().toStdString();
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
