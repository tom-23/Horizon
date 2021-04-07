#ifndef REGION_H
#define REGION_H

//#include <QGraphicsItem>

#include <QString>


// #include "LabSound/LabSound.h"
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


class Region
{
public:
    Region(Timeline *_timeline, Track *_track, QString uuid);
    ~Region();

    virtual Track* getTrack();
    virtual Timeline* getTimeline();
    virtual RegionGraphicItem* getRegionGraphicItem();
    virtual void setRegionGraphicItem(RegionGraphicItem *rgi);
    virtual void setTrack(Track *_track);

    virtual void setGridLocation(double time);
    virtual double getGridLocation();

    virtual void setGridLength(double value);
    virtual double getGridLength();

    virtual float getGain();
    virtual void setGain(float _gain);

    virtual QString getRegionName();
    virtual void setRegionName(QString _name);

    virtual QString getUUID();

    virtual bool getSelected();
    virtual void setSelected(bool _selected);

protected:



    Track *track;
    Timeline *timeline;
    RegionGraphicItem *regionGraphicsItem;

    double gridLocation;
    double length;

    QString regionName;

    float gain;

    QString uuid;

    bool selected;

};

#include "track.h"

#endif // REGION_H
