#include "filerendering.h"

Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr)
Q_DECLARE_METATYPE(std::shared_ptr<AudioContext>)
Q_DECLARE_METATYPE(AudioStreamConfig)
Q_DECLARE_METATYPE(std::string);

void FileRenderingThread::doWork(AudioManager *audioMan, AudioStreamConfig config, std::string fileName) {
    debug::out(3, "Spawned file render thread");
    debug::out(3, "Starting file Rendering...");

    lab::AudioContext& ac = *audioMan->context.get();

    audioMan->context = lab::MakeOfflineAudioContext(config, 60000.f);

    auto recorder = std::make_shared<RecorderNode>(ac, config);

    audioMan->context->addAutomaticPullNode(recorder);
    recorder->startRecording();


    debug::out(3, "Starting offline playback...");
    audioMan->context->connect(recorder, audioMan->getOutputNode());

    audioMan->play();



    audioMan->context->offlineRenderCompleteCallback = [&recorder, config, this, audioMan, fileName] {
        recorder->stopRecording();
        audioMan->context->removeAutomaticPullNode(recorder);
        //context->removeAutomaticPullNode(recorder);
        debug::out(3, "Writing to wav file...");
        recorder->writeRecordingToWav(fileName, false);
        debug::out(3, "All done!");

        emit this->resultReady();
    };

    audioMan->context->startOfflineRendering();
}



FileRendering::FileRendering(QObject *parent, std::function<void()> _callback) : QObject(parent)
{
    qRegisterMetaType<std::shared_ptr<AudioContext>>();
    qRegisterMetaType<AudioStreamConfig>();
    qRegisterMetaType<std::string>();

    callback = _callback;
    FileRenderingThread *rt = new FileRenderingThread;
    rt->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, rt, &QObject::deleteLater);
    connect(this, &FileRendering::operate, rt, &FileRenderingThread::doWork);
    connect(rt, &FileRenderingThread::resultReady, this, &FileRendering::handleResults, Qt::QueuedConnection);
    workerThread.start();
}

FileRendering::~FileRendering() {
    workerThread.quit();
    workerThread.wait();
}

void FileRendering::handleResults() {
    callback();
}


