#include "track.h"

Track::Track(Timeline *_timeLine, AudioManager *_audioMan) {

    audioMan = _audioMan;
    timeline = _timeLine;
    context = audioMan->getAudioContext();
    trackInputNode = std::make_shared<GainNode>();
    trackOutputNode = std::make_shared<GainNode>();
    analyser = std::make_shared<AnalyserNode>();
    trackInputNode->gain()->setValue(1.0f);
    trackOutputNode->gain()->setValue(1.0f);
    context->connect(analyser, trackInputNode);
    context->connect(trackOutputNode, analyser);
    context->connect(audioMan->getOutputNode(), trackOutputNode);
    selected = false;
    regionList = new std::vector<class Region *>;

    gain = 1.0f;
    setMute(false);

}

Track::~Track() {
    context->disconnect(audioMan->getOutputNode(), trackOutputNode);
    context->disconnect(trackInputNode, trackOutputNode);
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

void Track::switchRegion(Region *_region, Track *newTrack) {

    regionList->erase(regionList->begin() + getIndexByRegion(_region));
    context->disconnect(trackInputNode, _region->getOutputNode());

    newTrack->setRegion(_region);
    _region->disconnectTrack();
    _region->setTrack(newTrack);
}

AudioManager* Track::getAudioManager() {
    return audioMan;
}

std::shared_ptr<AudioContext> Track::getAudioContext() {
    return audioMan->getAudioContext();
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
}

bool Track::getMute() {
    return mute;
}

bool Track::getSolo() {
    return solo;
}
