#include "fileloading.h"

Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr)
Q_DECLARE_METATYPE(std::shared_ptr<AudioBus>)
Q_DECLARE_METATYPE(std::shared_ptr<SampledAudioNode>)
Q_DECLARE_METATYPE(std::vector<const float *>)

void FileLoadingThread::doWork(AudioManager *audioManager, QString loadedFileName) {
    debug::out(3, "Spawned file handling thread");
    debug::out(3, "Starting file loading...");
    std::shared_ptr<AudioBus> audioClipBus = audioManager->MakeBusFromSampleFile(loadedFileName.toStdString());

    std::shared_ptr<SampledAudioNode> audioClipNode = std::make_shared<SampledAudioNode>();
    {
        ContextRenderLock r(audioManager->context.get(), "Horizon");
        audioClipNode->setBus(r, audioClipBus);
    }


    debug::out(3, "Loaded audio, running callback function...");

    emit resultReady(audioClipBus, audioClipNode, audioManager->getPeaks(audioClipBus));
}



FileLoading::FileLoading(QObject *parent, std::function<void()> _callback) : QObject(parent)
{
    qRegisterMetaType<std::shared_ptr<AudioBus>>();
    qRegisterMetaType<std::shared_ptr<SampledAudioNode>>();
    qRegisterMetaType<std::vector<const float *>>();
    callback = _callback;
    FileLoadingThread *flt = new FileLoadingThread;
    flt->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, flt, &QObject::deleteLater);
    connect(this, &FileLoading::operate, flt, &FileLoadingThread::doWork);
    connect(flt, &FileLoadingThread::resultReady, this, &FileLoading::handleResults, Qt::QueuedConnection);
    workerThread.start();
}

FileLoading::~FileLoading() {
    workerThread.quit();
    workerThread.wait();
}

void FileLoading::handleResults(std::shared_ptr<AudioBus> _bus, std::shared_ptr<SampledAudioNode> _node, std::vector<const float *> _peaks) {
    bus = _bus;
    node = _node;
    peaks = _peaks;
    callback();
}


