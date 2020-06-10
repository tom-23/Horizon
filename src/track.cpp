#include "track.h"
#include <iostream>

Track::Track(Timeline *_timeLine, AudioManager *_audioMan) {

    audioMan = _audioMan;
    timeline = _timeLine;
    context = audioMan->getAudioContext();
    trackNode = new std::shared_ptr<GainNode>;
    selected = false;
    regionList = new std::vector<class Region *>;

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

AudioManager* Track::getAudioManager() {
    return audioMan;
}

std::shared_ptr<AudioContext>* Track::getAudioContext() {
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

//void Track::removeRegion(int position) {
//    std::vector<class Region *>::iterator it = std::find(regionList->begin(), regionList->end(), _region);
//    if (it != regionList->end()) {
//
//        regionList->erase(std::distance(regionList->begin(), it));
//    }
//}
