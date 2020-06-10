#include "region.h"

Region::Region(Timeline *_timeline, Track *_track)
{
    timeline = _timeline;
    track = _track;
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
