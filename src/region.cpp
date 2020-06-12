#include "region.h"


Region::Region(Timeline *_timeline, Track *_track)
{
    timeline = _timeline;
    track = _track;
    context = track->getAudioContext();
    outputNode = std::make_shared<GainNode>();
    outputNode->gain()->setValue(1.0f);
    context->connect(track->getTrackInputNode(), outputNode);
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

void Region::setTrack(Track *_track) {
    if (track != nullptr) {
       context->disconnect(track->getTrackOutputNode(), outputNode);
    }
    context->connect(_track->getTrackOutputNode(), outputNode);
    track = _track;
}

double Region::getGridLocation() {
    return gridLocation;
}

void Region::setGridLocation(double time) {
    gridLocation = time;
}

double Region::getGridLength() {
    return length;
}

void Region::setGridLength(double value) {
    length = value;
}
