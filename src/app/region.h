#ifndef REGION_H
#define REGION_H

//#include <QGraphicsItem>


#include "LabSound/LabSound.h"
//#include <QString>
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
    Region(Timeline *_timeline, Track *_track, std::string uuid);
    ~Region();

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

    virtual std::string getRegionName();
    virtual void setRegionName(std::string _name);

    virtual std::shared_ptr<GainNode> getOutputNode();

    virtual std::string getUUID();

protected:



    Track *track;
    Timeline *timeline;
    RegionGraphicItem *regionGraphicsItem;

    std::shared_ptr<GainNode> outputNode;

    double gridLocation;
    double length;

    std::string regionName;

    float gain;

    std::string uuid;

};

#include "track.h"

#endif // REGION_H
