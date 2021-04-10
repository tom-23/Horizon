#include "track.h"

Track::Track(Timeline &_timeLine, AudioManager &_audioMan, QString _uuid) {

    debug::out(3, "Creating track");
    audioMan = &_audioMan;

    debug::out(3, "setting timeline");
    timeline = &_timeLine;

    uuid = _uuid;

    selected = false;
    regionList = new std::vector<class Region *>;
    selectedRegionList = new std::vector<class Region *>;

    peakdB = -100;
    setMute(false);
    setGain(0.0);
    setPan(0.0);
}

Track::~Track() {

    for (auto r : *regionList) {
        delete r;
    }
    delete trackControlWidget;
    delete mixerChannelWidget;
    delete trackGraphicItem;

    audioMan->sendCommand("deleteTrack", uuid);
}

void Track::setTrackControlsWidget(TrackControlsWidget *_tcw) {
    trackControlWidget = _tcw;

}

void Track::setMixerChannelWidget(MixerChannelWidget *_mcw) {
    mixerChannelWidget = _mcw;
}

void Track::setTrackGraphicsItem(TrackGraphicItem *_tgi) {
    trackGraphicItem = _tgi;
}

AudioRegion* Track::addAudioRegion(QString regionUUID) {

    AudioRegion *audioRegion = new AudioRegion(timeline, this, regionUUID);
    regionList->push_back(audioRegion);
    audioMan->sendCommand("addAudioRegion", uuid, regionUUID);
    return audioRegion;
}

void Track::setRegion(Region *_region) {

    regionList->insert(regionList->end(), _region);
}

void Track::removeRegion(Region *_region) {

    regionList->erase(regionList->begin() + getIndexByRegion(_region));
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
    mixerChannelWidget->setSelected(selected);
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

//void Track::removeRegion(int position) {
//    std::vector<class Region *>::iterator it = std::find(regionList->begin(), regionList->end(), _region);
//    if (it != regionList->end()) {
//
//        regionList->erase(std::distance(regionList->begin(), it));
//    }
//}

void Track::setGain(float _value) {
    gain = pow(10, (_value / 20));
    gainNonLog = _value;
    audioMan->sendCommand("setTrackGain", uuid, _value);
}

float Track::getGain() {

    return gainNonLog;
}

void Track::setPan(float _value) {
    pan = _value;
    audioMan->sendCommand("setTrackPan", uuid, _value);
}

float Track::getPan() {
    return pan;
}

void Track::setMute(bool _mute) {
    mute = _mute;
    audioMan->sendCommand("setTrackMute", uuid, _mute);
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

void Track::setColor(QColor _color) {
    color = _color;
}

void Track::updateColor(QColor _color) {
    color = _color;
    trackControlWidget->updateColor();
    mixerChannelWidget->updateColor();
}

int Track::getAudioRegionListCount() {
    return regionList->size();
}

AudioRegion* Track::getAudioRegionByIndex(int index) {
    return dynamic_cast<AudioRegion*>(regionList->at(index));
}

QString Track::getUUID() {
    return uuid;
}

void Track::uiUpdate() {
    trackControlWidget->uiUpdate();
    mixerChannelWidget->uiUpdate();
}

Region* Track::getSelectedRegion(int index) {
    if (selectedRegionList->size() != 0) {
        return selectedRegionList->at(index);
    } else {
        return nullptr;
    }
}

void Track::setRegionSelected(Region *region, bool selected) {
    if (selected == true) {
        debug::out(3, "Pushing region to vector...");
        for(int t = 0; t < audioMan->getTrackListCount(); t++) {
            Track *track = audioMan->trackList->at(t);
            for (int i = 0; i < int(track->selectedRegionList->size()); i++ ) {
                track->setRegionSelected(track->selectedRegionList->at(i), false);
            }
        }

        selectedRegionList->clear();
        selectedRegionList->push_back(region);
        debug::out(3, "Setting region as selected...");
        region->setSelected(true);
    } else {
        auto iterator = std::find(selectedRegionList->begin(), selectedRegionList->end(), region);

        if (iterator != selectedRegionList->end()) {
            int index = std::distance(selectedRegionList->begin(), iterator);
            selectedRegionList->erase(selectedRegionList->begin() + index);
            region->setSelected(false);
        }
    }
}

AudioEffect* Track::addAudioEffect(effectType type, QString uuid) {
    // this has very little reason to be here. It will probably get nuked one day but it's not doing harm at the moment.
    if (uuid == "") {
        uuid = "testUUID";
    }

    if (type == effectType::compressor) {

    }
    return nullptr;
}

bool Track::isLSilent() {
    return false;
}

bool Track::isRSilent() {
    return false;
}
