#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "LabSound/LabSound.h"
#include "audioregionmanager.h"
#include "trackaudio.h"
#include "audioutil.h"
#include <iostream>
#include <QtGui>


using namespace lab;


class AudioManager
{
public:
    AudioManager(QObject *_parent);
    void play();
    void pause();
    void stop();
    void updateSchedule();

    std::vector<class TrackAudio *> trackAudioList;
    std::vector<class AudioRegionManager *> audioRegionManagerList;
private:
    QObject *parent;
    std::shared_ptr<AudioContext> context;
    std::shared_ptr<GainNode> outputNode;
    QBasicTimer clock;
    void eventLoop(QTimerEvent *event);


};

#endif // AUDIOMANAGER_H
