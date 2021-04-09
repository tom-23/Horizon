#ifndef GETSHAREDMEMORY_H
#define GETSHAREDMEMORY_H

#include <QThread>
#include <QTimer>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QDataStream>
#include <QBuffer>
#include <QDataStream>

class AudioManager;

struct HorizonTrack {
    QString uuid;
    int leftChannelRMS = 0;
    int rightChannelRMS = 0;
    int leftChannelPWR = 0;
    int rightChannelPWR = 0;
};

struct HorizonData {
    double currentGridLocation;
    QList<HorizonTrack> tracks;
};


class GetSharedMemory : public QThread
{
public:
    GetSharedMemory();
    void run() override;
    AudioManager *audioManager;
private:
    QTimer *updateTimer;

    QSystemSemaphore readSemaphore;
    QSystemSemaphore writeSemaphore;
    QSharedMemory sharedMemory;
};


#endif // GETSHAREDMEMORY_H
