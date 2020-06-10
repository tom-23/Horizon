#ifndef REGION_H
#define REGION_H


#include "regiongraphicitem.h"

class Timeline;
class Track;
class RegionGraphicItem;

class Region
{
public:
    Region(Timeline *_timeline, Track *_track);

    Track* getTrack();
    Timeline* getTimeline();
    RegionGraphicItem* getRegionGraphicItem();
    void setRegionGraphicItem(RegionGraphicItem *rgi);

private:

    Track *track;
    Timeline *timeline;
    RegionGraphicItem *regionGraphicsItem;


};

#endif // REGION_H
