#include "audiomanager.h"



AudioManager::AudioManager(QObject *_parent)
{
    parent = _parent;
    const auto defaultAudioDeviceConfigurations = GetDefaultAudioDeviceConfiguration();
    context = lab::MakeRealtimeAudioContext(defaultAudioDeviceConfigurations.second, defaultAudioDeviceConfigurations.first);

    outputNode = std::make_shared<GainNode>();
    outputNode->gain()->setValue(0.0f);

    //context->connect(context->device(), outputNode);

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
