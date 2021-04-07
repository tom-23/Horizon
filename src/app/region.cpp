#include "region.h"


Region::Region(Timeline *_timeline, Track *_track, QString _uuid)
{
    // here we assign some default variables
    uuid = _uuid;
    timeline = _timeline;
    track = _track;
    // create a new gain node. We will use this as the output gain.
    //setGain(1.0f);
    // default to track position 1 and selected.
    gridLocation = 1;
    selected = false;
}

Region::~Region() {
    delete regionGraphicsItem;

}

Track* Region::getTrack() {
    return track;
}

Timeline* Region::getTimeline() {
    return timeline;
}

RegionGraphicItem* Region::getRegionGraphicItem() {
    return regionGraphicsItem;
}

void Region::setRegionGraphicItem(RegionGraphicItem *rgi) {
    regionGraphicsItem = rgi;
}

void Region::setTrack(Track *_track) {
    debug::out(3, "Switched Tracks");
    track->getAudioManager()->sendCommand("setRegionTrack", uuid, _track->getUUID());
    track = _track;
}

double Region::getGridLocation() {
    return gridLocation;
}

void Region::setGridLocation(double _gridLocation) {
    track->getAudioManager()->sendCommand("setRegionLocation", uuid, _gridLocation);
    gridLocation = _gridLocation;
    debug::out(3, "Grid location just set!");
}

double Region::getGridLength() {
    return length;
}

void Region::setGridLength(double _length) {
    track->getAudioManager()->sendCommand("setRegionLength", uuid, _length);
    length = _length;
}

void Region::setGain(float _gain) {
    track->getAudioManager()->sendCommand("setRegionGain", uuid, _gain);
    gain = _gain;
}

float Region::getGain() {
    return gain;
}


QString Region::getRegionName() {
    return regionName;
}

void Region::setRegionName(QString _name) {
    regionName = _name;
}

QString Region::getUUID() {
    return uuid;
}

bool Region::getSelected() {
    return selected;
}

void Region::setSelected(bool _selected) {
    selected = _selected;
    // also setting the region graphic item to selected.
    regionGraphicsItem->setSelected(_selected);
}
