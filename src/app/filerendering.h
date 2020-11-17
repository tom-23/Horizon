#ifndef FILERENDERING_H
#define FILERENDERING_H

#include <QObject>
#include <QThread>
#include "audiomanager.h"


class FileRenderingThread : public QObject {
    Q_OBJECT

public slots:
    void doWork(AudioManager *audioMan, AudioStreamConfig config, std::string fileName);
signals:
    void resultReady();
};


class FileRendering : public QObject
{
    Q_OBJECT

public:
    FileRendering(QObject *parent = nullptr, std::function<void()> callback = nullptr);
    ~FileRendering();

private:
    std::function<void()> callback;
    QThread workerThread;
public slots:
    void handleResults();
signals:
    void operate(AudioManager *audioMan, AudioStreamConfig config, std::string fileName);
};

#endif // FILERendering_H
