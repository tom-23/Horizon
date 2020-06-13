#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H


#include <iostream>
#include <stdio.h>
#include <chrono>
#include <ratio>
#include <thread>

#include "LabSound/LabSound.h"

class Metronome;

#include "metronome.h"




//#include "timer.h"

#include "track.h"
#include "region.h"
#include "audioregion.h"

#include "timeline.h"

#include "audioutil.h"
#include "timer.h"
#include "debug.h"

#include <iostream>
#include <stdio.h>
#include <QtGui>
#include <chrono>
#include <ratio>
#include <thread>

//class AudioTrackManager;
//class Track;

using namespace lab;
using namespace std::chrono_literals;

class AudioManager
{
public:
    AudioManager(Timeline *_timeline);

    void play();
    void pause();
    void stop();

    bool isPlaying;

    void updateSchedule();

    void setDivision(int _division);
    void setBPM(double _beatsPerMinuet);
    void setLookAhead(double _value);

    float getCurrentGridTime();
    void setCurrentGridTime(float _value);

    double gridTimeToContextSeconds(float _gridTime);
    double gridTimeToSeconds(float _gridTime);
    float secondsToGridTime(double _seconds);
    float getCurrentRelativeTime();

    Track* addTrack();
    Track* getTrackByIndex(int index);
    Track* getSelectedTrack(int index);
    std::vector<class Track*>* getSelectedTracks();
    void setTrackSelected(Track *track, bool selected);
    void setTrackRangeSelected(Track *firstTrack, Track *lastTrack);

    int getTrackListCount();
    void scheduleTracks();

    std::shared_ptr<AudioContext> getAudioContext();
    std::shared_ptr<GainNode> getOutputNode();

    std::shared_ptr<AudioBus> MakeBusFromSampleFile(std::string fileName);


    float startTime;
    float stopTime;

    std::string calculatePeaks(std::shared_ptr<AudioBus> bus);


private:
    QObject *parent;
    std::shared_ptr<AudioContext> context;
    std::shared_ptr<GainNode> outputNode;


    std::vector<class Track *> *trackList;
    std::vector<class Track *> *selectedTrackList;

    Metronome *metronome;




    Timeline *timeline;

    double bpm;
    double beatLength;
    double barLength;


    Timer *eventTimer;
    bool quitLoop;
    void eventLoop();

    int division;
    int currentPos;
    double lookAhead;

    float currentGridTime;



    bool scheduled;

     void updateMetSchedule();
    void cancelTrackPlayback();

};

#endif // AUDIOMANAGER_H
