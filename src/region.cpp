#include "region.h"


Region::Region(Timeline *_timeline, Track *_track)
{
    timeline = _timeline;
    track = _track;
    context = track->getAudioContext();
    outputNode = std::make_shared<GainNode>();
    setGain(1.0f);
    context->connect(track->getTrackInputNode(), outputNode);
    gridLocation = 1;
}

//Region::~Region() {
//    context->disconnect(track->getTrackOutputNode(), outputNode);
//}

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
    context->disconnect(track->getTrackInputNode(), outputNode);
    debug::out(3, "Disconnected from track");
}

void Region::setTrack(Track *_track) {
    context->connect(_track->getTrackInputNode(), outputNode);
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
