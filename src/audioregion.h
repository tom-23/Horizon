#ifndef AUDIOREGION_H
#define AUDIOREGION_H


//class Track;
//class Timeline;
//class Region;
class Timeline;
class Track;
class RegionGraphicItem;

#include "LabSound/LabSound.h"
#include "region.h"

using namespace lab;


class AudioRegion : public Region
{
public:
    AudioRegion(Timeline *_timeline, Track *_track);

    //void loadFile(std::string fileName);
    //void scheduleTime(double contextTime);

private:

    //std::shared_ptr<AudioBus> audioClip;
    //std::shared_ptr<SampledAudioNode> audioClipNode;

    //std::string loadedFileName;

    //double duration;
};

#endif // AudioRegion_H
