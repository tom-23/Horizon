#include "audioregionmanager.h"

AudioRegionManager::AudioRegionManager(std::shared_ptr<AudioContext> _context, std::shared_ptr<GainNode> _trackNode, int _trackID, int _regionID)
{
    context = _context;
    trackNode = _trackNode;
    trackID = _trackID;
    regionID = _regionID;
}

float AudioRegionManager::loadFile(std::string _audioFileName) {
    audioClip = MakeBusFromFile(_audioFileName, false);
    audioClipNode = std::make_shared<SampledAudioNode>();
    {
        ContextRenderLock r(context.get(), "Audio Region");
        audioClipNode->setBus(r, audioClip);
    }
    context->connect(regionGainNode, audioClipNode);
    context->connect(trackNode, regionGainNode);

    duration = audioClipNode->duration();
    return audioClipNode->duration();
}

void AudioRegionManager::setStartTime(float _time) {
    // audioClipNode->start(_time);
    std::shared_ptr<AudioChannel> ch;

    qDebug() << audioClip->channel(0)->data();
}

void AudioRegionManager::startFrom(float _time) {
    audioClipNode->startGrain(0.0f, _time);
}
