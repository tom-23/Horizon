#include "audiomanager.h"

AudioManager::AudioManager(QObject *_parent)
{
    parent = _parent;
    context = lab::Sound::MakeRealtimeAudioContext(lab::Channels::Stereo);

    outputNode = std::make_shared<GainNode>();
    outputNode->gain()->setValue(0.0f);

    context->connect(context->destination(), outputNode);

    TrackAudio *tn = new TrackAudio(context, outputNode, 0);
    AudioRegionManager *arm = new AudioRegionManager(context, tn->getTrackNode(), 0, 0);

    qDebug() << "Hey it worked!";
}

void AudioManager::eventLoop(QTimerEvent *event) {

}

void AudioManager::play() {
    clock.start(10, parent);
    updateSchedule();
}

void AudioManager::pause() {

}

void AudioManager::stop() {

}

void AudioManager::updateSchedule() {

}
