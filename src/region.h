#ifndef REGION_H
#define REGION_H

#include <QGraphicsItem>

class Timeline;
class Track;
class RegionGraphicItem;
//class AudioManager;


#include "audiomanager.h"
#include "regiongraphicitem.h"

using namespace lab;

class Region
{
public:
    Region(Timeline *_timeline, Track *_track);
    //~Region();

    Track* getTrack();
    Timeline* getTimeline();
    RegionGraphicItem* getRegionGraphicItem();
    void setRegionGraphicItem(RegionGraphicItem *rgi);
    void setTrack(Track *_track);

    //void disconnectTrack();

private:



    Track *track;
    Timeline *timeline;
    RegionGraphicItem *regionGraphicsItem;

    std::shared_ptr<AudioContext> context;
    std::shared_ptr<GainNode> outputNode;


};

#endif // REGION_H
