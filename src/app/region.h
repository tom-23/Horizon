#ifndef REGION_H
#define REGION_H

//#include <QGraphicsItem>


#include "LabSound/LabSound.h"
//#include "audioregion.h"


//class Timeline;
//class Track;
//class Region;
//class RegionGraphicItem;
//class AudioManager;

//#include "track.h"

class AudioManager;
//#include "audiomanager.h"
class Track;


class Timeline;
class RegionGraphicItem;
//#include "regiongraphicitem.h"

using namespace lab;

class Region
{
public:
    Region(Timeline *_timeline, Track *_track);
    //~Region();

    virtual Track* getTrack();
    virtual Timeline* getTimeline();
    virtual RegionGraphicItem* getRegionGraphicItem();
    virtual void setRegionGraphicItem(RegionGraphicItem *rgi);
    virtual void setTrack(Track *_track);

    virtual void disconnectTrack();

    virtual void setGridLocation(double time);
    virtual double getGridLocation();

    virtual void setGridLength(double value);
    virtual double getGridLength();

    virtual void schedule();

    virtual float getGain();
    virtual void setGain(float _gain);

    virtual std::shared_ptr<GainNode> getOutputNode();

protected:



    Track *track;
    Timeline *timeline;
    RegionGraphicItem *regionGraphicsItem;

    std::shared_ptr<GainNode> outputNode;

    double gridLocation;
    double length;

    float gain;

};

#include "track.h"

#endif // REGION_H
