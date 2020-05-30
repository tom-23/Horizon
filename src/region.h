#ifndef REGION_H
#define REGION_H

class Timeline;
class RegionGraphicItem;

class Region
{
public:
    Region(int _trackIndex, Timeline *_timeline, RegionGraphicItem *_regionGraphicsItem);
    int getTrackIndex();

    RegionGraphicItem* getRegionGraphicItem();

private:
    int trackIndex;
    RegionGraphicItem *regionGraphicsItem;
};

#endif // REGION_H
