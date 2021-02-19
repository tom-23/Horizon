#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H


#include <iostream>
#include <stdio.h>
#include <chrono>
#include <ratio>
#include <thread>
#include <math.h>

#include "LabSound/LabSound.h"

class Metronome;

#include "metronome.h"




//#include "timer.h"

#include "track.h"
#include "region.h"
#include "audioregion.h"

#include "gui/timeline.h"
#include "gui/mixer.h"

#include "common/audioutil.h"
#include "common/timer.h"
#include "common/debug.h"
#include "common/dialogs.h"
#include "common/util.h"
#include "filerendering.h"
#include "common/timerex.h"

#include "network/session.h"

#include <iostream>
#include <stdio.h>
#include <QtGui>
#include <chrono>
#include <ratio>
#include <thread>
#include <memory>

#include <QJsonObject>
#include <QJsonDocument>

#include <QThread>
#include <QUuid>

//class AudioTrackManager;
//class Track;

class Session;
class AudioRegion;

using namespace lab;
//using namespace std::chrono_literals;

class AudioManager
{
public:
    AudioManager(QWidget *parent, Timeline &_timeline);

    void initContext();

    void play();
    void pause();
    void stop();

    bool isPlaying;

    void updateSchedule();

    void setDivision(int _division);
    void setBPM(double _beatsPerMinuet);
    double getBPM();

    // TODO: this function at the moment is completely and utterly fucking useless.
    void setLookAhead(double _value);

    float getCurrentGridTime();
    void setCurrentGridTime(float _value);

    double gridTimeToContextSeconds(float _gridTime);
    double gridTimeToSeconds(float _gridTime);
    float secondsToGridTime(double _seconds);
    float getCurrentRelativeTime();

    Track* addTrack(std::string trackUUID);
    void removeTrack(Track *track);
    Track* getTrackByIndex(int index);

    Track* getSelectedTrack(int index);
    std::vector<class Track*>* getSelectedTracks();
    void setTrackSelected(Track *track, bool selected);
    void setTrackRangeSelected(Track *firstTrack, Track *lastTrack);

    int getTrackListCount();
    void scheduleTracks();

    std::shared_ptr<GainNode> getOutputNode();

    std::shared_ptr<AudioBus> MakeBusFromSampleFile(std::string fileName);


    // TODO: this should be private
    float startTime;
    float stopTime;

    std::vector<const float *> getPeaks(std::shared_ptr<AudioBus> bus);

    std::shared_ptr<AudioContext> context;

    // uh?
    void engageSolo();
    void disengageSolo();

    bool soloEnabled;

    void clearAll();

    Session *session;

    void moveRegion(QString uuid, double gridLocation);
    void setTrackMute(QString uuid, bool mute);
    void setTrackPan(QString uuid, float pan);
    void setTrackGain(QString uuid, float gain);

    Track* getTrackByUUID(QString uuid);
    AudioRegion* getAudioRegionByUUID(QString uuid);

    void renderAudio(QObject *parent, std::string fileName, int sampleRate, int channels);

    bool rendering;

    void eventLoop();


private:
    QObject *parent;

    std::shared_ptr<GainNode> outputNode;


    std::vector<class Track *> *trackList;
    std::vector<class Track *> *selectedTrackList;

    std::vector<class Region *> *selectedRegionList;

    Metronome *metronome;

    Timeline *timeline;

    double bpm;
    double beatLength;
    double barLength;


    TimerEX *eventTimer;
    bool quitLoop;


    int division;
    /*
     * currentPos is:
     * barNumber.division
     * In the future, I could implement a time-based grid but that would take time.
     */
    int currentPos;
    double lookAhead;

    float currentGridTime;
    bool scheduled;

    void updateMetSchedule();
    void cancelTrackPlayback();


};

#endif // AUDIOMANAGER_H
