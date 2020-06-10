#ifndef AudioRegion_H
#define AudioRegion_H
#include "LabSound/LabSound.h"
//#include "audiomanager.h"
#include "region.h"
#include "track.h"
#include "timeline.h"
//class Region;

using namespace lab;

class AudioRegion : public Region
{
public:
    explicit  AudioRegion(Timeline *_timeline, Track *_track);

    void loadFile(std::string fileName);
    void scheduleTime(double contextTime);
private:


    std::shared_ptr<AudioContext> context;
    std::shared_ptr<GainNode> trackNode;

    std::shared_ptr<AudioBus> audioClip;
    std::shared_ptr<SampledAudioNode> audioClipNode;
    std::shared_ptr<GainNode> regionGainNode;

    std::string loadedFileName;

    double duration;
};

#endif // AudioRegion_H
