#ifndef AUDIOREGION_H
#define AUDIOREGION_H



//class Timeline;
//class Track;
//class Region;
//class RegionGraphicItem;

//#include "LabSound/LabSound.h"
//#include "track.h"
#include "track.h"
//class Region;
#include "region.h"
//class Track;
//class Timeline;

using namespace lab;


class AudioRegion : public Region
{
public:
    AudioRegion(Timeline *_timeline, Track *_track);

    void loadFile(std::string fileName);
    void schedule() override;
    void cancelSchedule();

    void disconnectTrack() override;
    void setTrack(Track *_track) override;

    std::string getLoadedFileName();

private:

    std::shared_ptr<AudioBus> audioClipBus;
    std::shared_ptr<SampledAudioNode> audioClipNode;

    std::string loadedFileName;

    void loadFileThread(std::function<void()> callback);
    void loadedFileCallBack();
    //double duration;

};



#endif // AudioRegion_H
