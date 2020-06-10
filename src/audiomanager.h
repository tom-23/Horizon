#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H


#include "LabSound/LabSound.h"




#include <iostream>
#include <stdio.h>
#include <QtGui>
#include <chrono>
#include <ratio>
#include <thread>


class Metronome;

#include "metronome.h"

#include "track.h"
#include "timeline.h"
#include "region.h"
//#include "audioregion.h"

#include "audioutil.h"
#include "timer.h"

#include "debug.h"


//class AudioTrackManager;
//class Track;
class Region;
class AudioRegion;



using namespace lab;

using namespace std::chrono_literals;




class AudioManager
{
public:
    AudioManager(Timeline *_timeline);

    void play();
    void pause();
    void stop();
    void updateSchedule();

    void setDivision(int _division);
    void setBPM(double _beatsPerMinuet);
    void setLookAhead(double _value);

    float getCurrentGridTime();
    void setCurrentGridTime(float _value);

    double gridTimeToContextSeconds(float _gridTime);



    Track* addTrack();
    Track* getTrackByIndex(int index);
    Track* getSelectedTrack(int index);
    std::vector<class Track*>* getSelectedTracks();
    void setTrackSelected(Track *track, bool selected);
    void setTrackRangeSelected(Track *firstTrack, Track *lastTrack);

    int getTrackListCount();

    std::shared_ptr<AudioContext> getAudioContext();
    std::shared_ptr<GainNode> getOutputNode();

    std::shared_ptr<AudioBus> MakeBusFromSampleFile(std::string fileName);




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

    double startTime;
    double stopTime;

    bool scheduled;

     void updateMetSchedule();

};

#endif // AUDIOMANAGER_H
