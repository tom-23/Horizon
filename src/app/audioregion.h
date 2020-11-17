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
#include "fileloading.h"
class FileLoading;
//class Timeline;

//#include <QThread>

using namespace lab;


class AudioRegion : public Region
{
public:
    AudioRegion(Timeline *_timeline, Track *_track, std::string uuid);

    void loadFile(std::string fileName, bool progressDialog);
    void schedule() override;
    void cancelSchedule();

    void disconnectTrack() override;
    void setTrack(Track *_track) override;

    void switchContext(AudioContext *context);

    std::string getLoadedFileName();
    std::string preLoadedFile;

private:

    std::shared_ptr<AudioBus> audioClipBus;
    std::shared_ptr<SampledAudioNode> audioClipNode;

    std::string loadedFileName;


    //void loadFileThread(std::function<void()> callback);
    void loadedFileCallBack();

    std::queue<std::function<void()>> callbackQueue;

    FileLoading *fileLoading;

    bool progressDialog;
    //double duration;

};



#endif // AudioRegion_H
