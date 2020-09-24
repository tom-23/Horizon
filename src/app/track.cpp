#include "track.h"

Track::Track(Timeline &_timeLine, AudioManager &_audioMan) {

    debug::out(3, "Creating track");
    audioMan = &_audioMan;
    debug::out(3, "setting timeline");
    timeline = &_timeLine;
    debug::out(3, "setting input node");
    trackInputNode = std::make_shared<GainNode>();
    debug::out(3, "setting output node");
    trackOutputNode = std::make_shared<GainNode>();
    analyser = std::make_shared<AnalyserNode>();
    trackInputNode->gain()->setValue(1.0f);
    trackOutputNode->gain()->setValue(1.0f);


    audioMan->context.get()->connect(analyser, trackInputNode);
    audioMan->context->connect(trackOutputNode, analyser);
    audioMan->context->connect(audioMan->getOutputNode(), trackOutputNode);

    selected = false;
    regionList = new std::vector<class Region *>;


    gain = 1.0f;
    peakdB = -100;
    setMute(false);



}

Track::~Track() {
    audioMan->context->disconnect(audioMan->getOutputNode(), trackOutputNode);
    audioMan->context->disconnect(trackInputNode, trackOutputNode);
}

void Track::setTrackControlsWidget(TrackControlsWidget *_tcw) {
    trackControlWidget = _tcw;
}

void Track::setTrackGraphicsItem(TrackGraphicItem *_tgi) {
    trackGraphicItem = _tgi;
}

void Track::setHScaleFactor(int _hScaleFactor) {

}

AudioRegion* Track::addAudioRegion() {

    AudioRegion *audioRegion = new AudioRegion(timeline, this);
    regionList->push_back(audioRegion);
    return audioRegion;
}

void Track::setRegion(Region *_region) {

    regionList->insert(regionList->end(), _region);
}

void Track::removeRegion(Region *_region) {

    qDebug() << "Removing Region... IDX" << index;
    regionList->erase(regionList->begin() + getIndexByRegion(_region));
    trackInputNode->uninitialize();
    analyser->uninitialize();
    qDebug() << "Track connections before:" << trackInputNode->numberOfInputs();
    audioMan->context->disconnect(trackInputNode, _region->getOutputNode());

    qDebug() << "Track connections after:" << trackInputNode->numberOfInputs();
    trackInputNode->initialize();

    analyser->initialize();
}

AudioManager* Track::getAudioManager() {
    return audioMan;
}

int Track::getIndex() {
    return index;
}

void Track::setIndex(int _index) {
    index = _index;
}

void Track::setSelected(bool _selected) {
    selected = _selected;
    trackControlWidget->setSelected(selected);
}

bool Track::getSelected() {
    return selected;
}

int Track::getIndexByRegion(Region *region) {
    auto iterator = std::find(regionList->begin(), regionList->end(), region);

    if (iterator != regionList->end()) {
       return std::distance(regionList->begin(), iterator);
    } else {
        return -1;
    }

}

std::shared_ptr<GainNode> Track::getTrackInputNode() {
    return trackInputNode;
}

std::shared_ptr<GainNode> Track::getTrackOutputNode() {
    return trackOutputNode;
}

void Track::scheduleAudioRegions() {
    for (int i = 0; i < int(regionList->size()); i++) {
        AudioRegion* audioRegion = dynamic_cast<AudioRegion*>(regionList->at(i));
        //double contextLocation = audioMan->gridTimeToContextSeconds(audioRegion->getGridLocation()) - audioMan->getCurrentRelativeTime();
        audioRegion->schedule();
        debug::out(3, "Scheduled a region...");
    }
}

void Track::cancelAudioRegions() {
    for (int i = 0; i < int(regionList->size()); i++) {
        AudioRegion* audioRegion = dynamic_cast<AudioRegion*>(regionList->at(i));
        audioRegion->cancelSchedule();
        debug::out(3, "Cancelling a region...");
    }
}

//void Track::removeRegion(int position) {
//    std::vector<class Region *>::iterator it = std::find(regionList->begin(), regionList->end(), _region);
//    if (it != regionList->end()) {
//
//        regionList->erase(std::distance(regionList->begin(), it));
//    }
//}

void Track::setGain(float _value) {
    gain = _value;
}

float Track::getGain() {
    return gain;
}

void Track::setMute(bool _mute) {
    mute = _mute;
    if (mute == true) {
        trackOutputNode->gain()->setValue(0.0f);
    } else {
        trackOutputNode->gain()->setValue(gain);
    }
}

void Track::setSolo(bool _solo) {
    solo = _solo;
    if (solo == true) {

    }
}

bool Track::getMute() {
    return mute;
}

bool Track::getSolo() {
    return solo;
}

QColor Track::getColor() {
    return color;
}

void Track::setColor(QColor *_color) {
    color = *_color;

}

int Track::getMeterData() {

    std::vector<float> buffer(2048);
    analyser->getFloatTimeDomainData(buffer);
    //analyser->getFloatFrequencyData(buffer);
   // qDebug() << "BUFFER" << buffer[0];

    float sumOfSquares = 0;
    for (int i = 0; i < (int)buffer.size(); i++) {
        sumOfSquares += pow(buffer[i], 2);

    }
    //qDebug() << "SOS" << sumOfSquares;

    float avgPowerDecibels = 10 * log10(sumOfSquares / buffer.size());
    int peakInstantaneousPower = 0;

    for (int i = 0; i < buffer.size(); i++) {
          int power = pow(buffer[i], 2);
          peakInstantaneousPower = max(power, peakInstantaneousPower);
    }

    float peakInstantaneousPowerDecibels = 10 * log10(peakInstantaneousPower);

    if (avgPowerDecibels >= peakdB) {
        peakdB = std::ceil(avgPowerDecibels * 100.0) / 100.0;

    }


    return round(avgPowerDecibels);

}

int Track::getAudioRegionListCount() {
    return regionList->size();
}

AudioRegion* Track::getAudioRegionByIndex(int index) {
    return dynamic_cast<AudioRegion*>(regionList->at(index));
}

