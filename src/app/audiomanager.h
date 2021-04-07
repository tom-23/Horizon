#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H


#include <iostream>
#include <stdio.h>
#include <chrono>
#include <ratio>
#include <thread>
#include <math.h>

//#include "timer.h"

#include "track.h"
#include "region.h"
#include "audioregion.h"
#include "audioschedulingthread.h"

#include "gui/timeline.h"
#include "gui/mixer.h"

#include "common/timer.h"
#include "common/debug.h"
#include "common/dialogs.h"
#include "common/util.h"
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
#include <QLocalSocket>

//class AudioTrackManager;
//class Track;

class Session;
class AudioRegion;
class AudioSchedulingThread;

//using namespace std::chrono_literals;

class AudioManager
{
public:
    AudioManager(QWidget *parent, Timeline &_timeline);

    void initSocket();
    void sendCommand(QString command);
    void sendCommand(QString command, QJsonValue value);
    void sendCommand(QString command, QJsonValue value1, QJsonValue value2);

    void play();
    void pause();
    void stop();

    bool isPlaying;

    void setDivision(int _division);
    void setBPM(double _beatsPerMinuet);
    double getBPM();

    float getCurrentGridTime();
    void setCurrentGridTime(float _value);

    double gridTimeToContextSeconds(float _gridTime);
    double gridTimeToSeconds(float _gridTime);
    float secondsToGridTime(double _seconds);
    float getCurrentRelativeTime();

    Track* addTrack(QString trackUUID);
    void removeTrack(Track *track);

    Track* getSelectedTrack(int index);
    std::vector<class Track*>* getSelectedTracks();
    void setTrackSelected(Track *track, bool selected);
    void setTrackRangeSelected(Track *firstTrack, Track *lastTrack);

    int getTrackListCount();


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


    bool rendering;

    std::vector<class Track *> *trackList;
    std::vector<class Track *> *selectedTrackList;

private:
    QWidget *parent;

    QLocalSocket *socket;
    void writeString();

    std::vector<class Region *> *selectedRegionList;

    Timeline *timeline;

    double bpm = 130.0;
    double beatLength;
    double barLength;

    int division = 4;
    /*
     * currentPos is:
     * barNumber.division
     * In the future, I could implement a time-based grid but that would take time.
     */
    int currentPos;

    float currentGridTime;
    bool scheduled;


    QList<QString> *dataQueue = new QList<QString>;

    void socketReadReady();

    void sendConfirmation();

    QDataStream dataStream;
    int connectionCount = 0;
    quint32 blockSize = 0;

};

#endif // AUDIOMANAGER_H
