#include "audiomanager.h"


AudioManager::AudioManager(QWidget *_parent, Timeline &_timeline)
{
    debug::out(3, "Timeline init");
    timeline = &_timeline;
    parent = _parent;

    // Initialising some variables that will be needed in later use
    isPlaying = false;
    currentGridTime = 1.0;

    debug::out(3, "Starting audio engine...");

    session = new Session(parent, *this);

    initSocket();

    trackList = new std::vector<class Track *>;
    selectedTrackList = new std::vector<class Track *>;

    debug::out(3, "Setting BPM");


    debug::out(3, "Audio engine started without any issues!");
    sendCommand("init");
    sendCommand("setBPM", bpm);
    sendCommand("setDivision", division);
}



/* initSocket
    Initalises a labsound real-time audio context
*/
void AudioManager::initSocket() {
    socket = new QLocalSocket(parent);
    //socket->setCurrentWriteChannel(0);
    socket->setServerName("HorizonAUMANEngine");
    dataStream.setDevice(socket);
    dataStream.setVersion(QDataStream::Qt_5_10);
    QObject::connect(socket, &QLocalSocket::readyRead, parent, [=] () { socketReadReady(); });
    socket->connectToServer();
}

void AudioManager::socketReadReady() {
    qDebug() << "Received Data";

    if (blockSize == 0) {
            // Relies on the fact that QDataStream serializes a quint32 into
            // sizeof(quint32) bytes
            if (socket->bytesAvailable() < (int)sizeof(quint32)) { return; };
            dataStream >> blockSize;
    }


    if (socket->bytesAvailable() < blockSize || dataStream.atEnd()) {
        return;
    }
    QString data;
    dataStream >> data;
    blockSize = 0;

    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());

    qDebug() << data;

    if (!doc.object().value("result").isUndefined()) {
        qDebug() << "Got Result";
        if (doc.object().value("result").toString() == "OK") {
            if (dataQueue->size() != 0) {
                dataQueue->pop_front();
                writeString();
            }
        }
        return;
    }

    if (doc.object().value("cmnd").toString() == "finishedAudioRegionLoad") {
        QString uuid = doc.object().value("value0").toString();
        getAudioRegionByUUID(uuid)->loadedFileCallBack(doc.object().value("value0").toDouble());
    }
    sendConfirmation();
}

void AudioManager::writeString() {
    if (dataQueue->size() == 0) {
        return;
    }
    //string = "TESTING";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    const QString &message = dataQueue->at(0);
    out << quint32(message.size());
    out << message;
    qDebug() << "Writing string" << message;
    socket->write(block);
    socket->flush();
}

void AudioManager::sendConfirmation() {
    qDebug() << "Sending confirmation";
    QJsonObject obj;
    obj.insert("result", "OK");
    QJsonDocument doc;
    doc.setObject(obj);
    dataQueue->push_back(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    if (dataQueue->size() == 1) {
        writeString();
    }
}

void AudioManager::sendCommand(QString command) {
    QJsonObject obj;
    obj.insert("cmnd", command);
    QJsonDocument doc;
    doc.setObject(obj);
    dataQueue->push_back(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    if (dataQueue->size() == 1) {
        writeString();
    }
}

void AudioManager::sendCommand(QString command, QJsonValue value) {
    QJsonObject obj;
    obj.insert("cmnd", command);
    obj.insert("value0", value);
    QJsonDocument doc;
    doc.setObject(obj);
    dataQueue->push_back(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    if (dataQueue->size() == 1) {
        writeString();
    }
}

void AudioManager::sendCommand(QString command, QJsonValue value, QJsonValue value1) {
    QJsonObject obj;
    obj.insert("cmnd", command);
    obj.insert("value0", value);
    obj.insert("value1", value1);
    QJsonDocument doc;
    doc.setObject(obj);
    dataQueue->push_back(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    if (dataQueue->size() == 1) {
        writeString();
    }
}

void AudioManager::play() {
    if (isPlaying == false) {
        sendCommand("play");
        isPlaying = true;
    }
}

void AudioManager::pause() {
    if (isPlaying == true) {
        isPlaying = false;
        sendCommand("pause");
    }
}

void AudioManager::stop() {

    if (isPlaying == true) {
        sendCommand("stop");
    }
}

void AudioManager::setDivision(int _division) {
    if (division != _division) {
        division = _division;
        sendCommand("setDivision", division);
    }
    barLength = bpm * division;
}

void AudioManager::setBPM(double _bpm) {
    if (bpm != _bpm) {
        sendCommand("setBPM", _bpm);
        bpm = _bpm;
    }
    beatLength = 60.00 / bpm;
    barLength = bpm * division;

    for (int t = 0; t < int(trackList->size()); ++t) {
        Track *track = trackList->at(t);
        for (int ar = 0; ar < track->getAudioRegionListCount(); ar++) {
            AudioRegion *audioRegion = track->getAudioRegionByIndex(ar);
            audioRegion->updateGridLength();
            audioRegion->getRegionGraphicItem()->setGridLength(audioRegion->getGridLength());
        }
    }

}

double AudioManager::getBPM() {
    return bpm;
}

float AudioManager::getCurrentGridTime() {
    return currentGridTime;
}

Track* AudioManager::addTrack(QString trackUUID) {
    debug::out(3, "Creating new track...");
    sendCommand("addTrack", trackUUID);
    Track *newTrack = new Track(*timeline, *this, trackUUID);
    trackList->push_back(newTrack);
    newTrack->setIndex(trackList->size() - 1);
    return newTrack;
}

void AudioManager::removeTrack(Track *track) {
    debug::out(3, "Deleting track");

    auto iterator = std::find(trackList->begin(), trackList->end(), track);
    if (iterator != trackList->end()) {
        int index = std::distance(trackList->begin(), iterator);
        trackList->erase(trackList->begin() + index);
    }
    delete track;
}

Track* AudioManager::getSelectedTrack(int index) {
    if (selectedTrackList->size() != 0) {
        return selectedTrackList->at(index);
    } else {
        return nullptr;
    }

}

std::vector<class Track*>* AudioManager::getSelectedTracks() {
    return selectedTrackList;
}

void AudioManager::setTrackSelected(Track *track, bool selected) {
    if (selected == true) {
        debug::out(3, "Pushing track to vector...");
        for (int i = 0; i < int(selectedTrackList->size()); i++ ) {
            setTrackSelected(selectedTrackList->at(i), false);
        }
        selectedTrackList->clear();
        selectedTrackList->push_back(track);
        debug::out(3, "Setting as selected...");
        track->setSelected(true);
    } else {
        auto iterator = std::find(selectedTrackList->begin(), selectedTrackList->end(), track);

        if (iterator != selectedTrackList->end()) {
            int index = std::distance(selectedTrackList->begin(), iterator);
            selectedTrackList->erase(selectedTrackList->begin() + index);
            track->setSelected(false);
        }
    }
}

void AudioManager::setTrackRangeSelected(Track *firstTrack, Track *lastTrack) {

    for (int i = 0; i < int(selectedTrackList->size()); i++ ) {
        setTrackSelected(selectedTrackList->at(i), false);
    }
    selectedTrackList->clear();

    auto firstIterator = std::find(trackList->begin(), trackList->end(), firstTrack);
    auto lastIterator = std::find(trackList->begin(), trackList->end(), lastTrack);

    int firstIndex;
    int lastIndex;

    if (firstIterator != trackList->end()) {
        firstIndex = std::distance(trackList->begin(), firstIterator);
    } else {
        return;
    }

    if (lastIterator != trackList->end()) {
        lastIndex = std::distance(trackList->begin(), lastIterator);
    } else {
        return;
    }

    for (int i = firstIndex; i < lastIndex; i++) {
        selectedTrackList->push_back(trackList->at(i));
        trackList->at(i)->setSelected(true);
    }
}

int AudioManager::getTrackListCount() {
    return trackList->size();
}

void AudioManager::setCurrentGridTime(float _value) {
    currentGridTime = _value;
}

void AudioManager::engageSolo() {
    soloEnabled = true;
    for (int i = 0; i < int(trackList->size()); i++) {
        if (trackList->at(i)->getSolo() == false) {
            //trackList->at(i)->getTrackOutputNode()->gain()->setValue(0.0f);
        }

    }
}

void AudioManager::disengageSolo() {
    soloEnabled = false;
    for (int i = 0; i < int(trackList->size()); i++) {
        if (trackList->at(i)->getSolo() == false) {
            //trackList->at(i)->getTrackOutputNode()->gain()->setValue(0.0f);
        }

    }
}

void AudioManager::clearAll() {
    for (auto p : *trackList) {
        delete p;
    }
    trackList->clear();
    selectedTrackList->clear();
    //selectedRegionList->clear();
}

Track* AudioManager::getTrackByUUID(QString uuid) {
    for (int ti= 0; ti < this->getTrackListCount(); ti++) {
        Track *track = trackList->at(ti);
        if (track->getUUID() == uuid) {
            return track;
        }
    }
    return nullptr;
}

AudioRegion* AudioManager::getAudioRegionByUUID(QString uuid) {
    for (int ti= 0; ti < this->getTrackListCount(); ti++) {
        Track *track = trackList->at(ti);
        for (int ri = 0; ri < track->getAudioRegionListCount(); ri++) {
            AudioRegion *audioRegion = track->getAudioRegionByIndex(ri);
            if (audioRegion->getUUID() == uuid) {
                return audioRegion;
            }
        }
    }
    return nullptr;
}

void AudioManager::moveRegion(QString uuid, double gridLocation) {
    AudioRegion *audioRegion = getAudioRegionByUUID(uuid);
    if (this->isPlaying == true) {
    }
    audioRegion->setGridLocation(gridLocation);
    audioRegion->getRegionGraphicItem()->setGridLocation(gridLocation);
    audioRegion->getRegionGraphicItem()->update();
}

void AudioManager::setTrackMute(QString uuid, bool mute) {
    Track *track = getTrackByUUID(uuid);
    track->setMute(mute);
}

void AudioManager::setTrackPan(QString uuid, float pan) {
    Track *track = getTrackByUUID(uuid);
    track->setPan(pan);
}

void AudioManager::setTrackGain(QString uuid, float gain) {
    Track *track = getTrackByUUID(uuid);
    track->setGain(gain);
}
