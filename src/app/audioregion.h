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

//class Timeline;

//#include <QThread>

class AudioRegion : public Region
{
public:
    AudioRegion(Timeline *_timeline, Track *_track, QString uuid);
    ~AudioRegion();

    void loadFile(QString fileName, bool progressDialog);


    QString getLoadedFileName();
    QString preLoadedFile;

    void updateGridLength();

    bool isScheduled;
    void loadedFileCallBack(double length);

private:

    QString loadedFileName;


    //void loadFileThread(std::function<void()> callback);


    bool progressDialog;

    AudioManager *audioManager;

    //double duration;

};



#endif // AudioRegion_H
