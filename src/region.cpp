#include "region.h"
#include "timeline.h"

Region::Region(int _trackIndex, Timeline *timeline, RegionGraphicItem *_regionGraphicsItem)
{
    trackIndex = _trackIndex;
    regionGraphicsItem = _regionGraphicsItem;
}

int Region::getTrackIndex() {
    return trackIndex;
}

RegionGraphicItem* Region::getRegionGraphicItem() {
    return regionGraphicsItem;
}

