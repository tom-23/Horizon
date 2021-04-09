#ifndef AUDIOSCHEDULINGTHREAD_H
#define AUDIOSCHEDULINGTHREAD_H

#include <QThread>
#include <QTimer>
#include "audiomanager.h"

//class Track;

class AudioSchedulingThread : public QThread
{
public:
    AudioSchedulingThread();
    std::vector<Track *> *trackList;
    void run() override;
private:
    QTimer *updateTimer = new QTimer(this);

};

#endif // AUDIOSCHEDULINGTHREAD_H
