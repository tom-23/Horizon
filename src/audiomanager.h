#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "LabSound/LabSound.h"
#include "audioregionmanager.h"
#include "trackaudio.h"
#include "audioutil.h"
#include "timer.h"
#include <iostream>
#include <stdio.h>
#include <QtGui>
#include <chrono>
#include <ratio>
#include <thread>

#include "timeline.h"


using namespace lab;

using namespace std::chrono_literals;




class AudioManager
{
public:
    AudioManager();

    void play();
    void pause();
    void stop();
    void updateSchedule();


    void setDivision(int _division);
    void setBPM(double _beatsPerMinuet);

    float getCurrentGridTime();
    void setCurrentGridTime(float _value);



    std::shared_ptr<AudioBus> MakeBusFromSampleFile(std::string fileName);

    std::vector<class TrackAudio *> trackAudioList;
    std::vector<class AudioRegionManager *> audioRegionManagerList;


private:
    QObject *parent;
    std::shared_ptr<AudioContext> context;
    std::shared_ptr<GainNode> outputNode;

    std::shared_ptr<AudioBus> metPrimaryBus;
    std::shared_ptr<SampledAudioNode> metPrimaryNode;

    std::shared_ptr<AudioBus> metSecondaryBus;
    std::shared_ptr<SampledAudioNode> metSecondaryNode;

    Timeline *timeline;

    double bpm;
    double beatLength;
    double barLength;


    Timer *eventTimer;
    bool quitLoop;
    void eventLoop();

    int division;
    int currentPos;

    float currentGridTime;

    double startTime;
    double stopTime;

};

#endif // AUDIOMANAGER_H
