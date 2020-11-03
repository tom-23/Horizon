#ifndef FILELOADING_H
#define FILELOADING_H

#include <QObject>
#include <QThread>
#include "audiomanager.h"


class FileLoadingThread : public QObject {
    Q_OBJECT

public slots:
    void doWork(AudioManager *audioManager, QString loadedFileName);
signals:
    void resultReady(std::shared_ptr<AudioBus> bus, std::shared_ptr<SampledAudioNode> node, std::vector<const float *> peaks);
};


class FileLoading : public QObject
{
    Q_OBJECT

public:
    FileLoading(QObject *parent = nullptr, std::function<void()> callback = nullptr);
    ~FileLoading();
    std::shared_ptr<AudioBus> bus;
    std::shared_ptr<SampledAudioNode> node;
    std::vector<const float *> peaks;
private:
    std::function<void()> callback;
    QThread workerThread;
public slots:
    void handleResults(std::shared_ptr<AudioBus> _bus, std::shared_ptr<SampledAudioNode> _node, std::vector<const float *> _peaks);
signals:
    void operate(AudioManager *audioManager, QString loadedFileName);
};

#endif // FILELOADING_H
