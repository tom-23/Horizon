#include "region.h"


Region::Region(Timeline *_timeline, Track *_track, std::string _uuid)
{
    uuid = _uuid;
    timeline = _timeline;
    track = _track;
    outputNode = std::make_shared<GainNode>();
    setGain(1.0f);
    track->getAudioManager()->context->connect(track->getTrackInputNode(), outputNode);
    gridLocation = 1;
    selected = false;
}

Region::~Region() {
    delete regionGraphicsItem;
    track->getAudioManager()->context->disconnect(track->getTrackOutputNode(), outputNode);
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

void Region::disconnectTrack() {
    track->getAudioManager()->context->disconnect(track->getTrackInputNode(), outputNode);
    debug::out(3, "Disconnected from track");
}

void Region::setTrack(Track *_track) {
    track->getAudioManager()->context->connect(_track->getTrackInputNode(), outputNode);
    debug::out(3, "Connected to track");
    setGain(gain);
    track = _track;
}

double Region::getGridLocation() {
    return gridLocation;
}

void Region::setGridLocation(double time) {
    gridLocation = time;
    debug::out(3, "Grid location just set!");
}

double Region::getGridLength() {
    return length;
}

void Region::setGridLength(double value) {
    length = value;
}

void Region::schedule() {

}

void Region::setGain(float _gain) {
    gain = _gain;
    outputNode->gain()->setValue(_gain);
}

float Region::getGain() {
    return gain;
}

std::shared_ptr<GainNode> Region::getOutputNode() {
    return outputNode;
}

std::string Region::getRegionName() {
    return regionName;
}

void Region::setRegionName(std::string _name) {
    regionName = _name;
}

std::string Region::getUUID() {
    return uuid;
}

bool Region::getSelected() {
    return selected;
}

void Region::setSelected(bool _selected) {
    selected = _selected;
    if (selected) {
        qDebug() << "REGION IS SELECTED";
    }
    regionGraphicsItem->setSelected(_selected);
}
