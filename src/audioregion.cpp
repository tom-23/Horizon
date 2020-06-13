#include "audioregion.h"


AudioRegion::AudioRegion(Timeline *_timeline, Track *_track) : Region(_timeline, _track)
{
    debug::out(3, "Audio region added");
}

void AudioRegion::loadFile(std::string fileName) {
    debug::out(3, "Begining file loading...");
    loadedFileName = fileName;
    audioClipBus = track->getAudioManager()->MakeBusFromSampleFile(loadedFileName);

    audioClipNode = std::make_shared<SampledAudioNode>();
    {
        ContextRenderLock r(track->getAudioContext().get(), "Horizon");
        audioClipNode->setBus(r, audioClipBus);
    }



    track->getAudioContext().get();
    context->connect(outputNode, audioClipNode);

    length = track->getAudioManager()->secondsToGridTime(audioClipNode->duration()) - 1;

    debug::out(3, "Length calculated");

    track->getAudioManager()->calculatePeaks(audioClipBus);


    regionGraphicsItem->setGridLength(length);

    debug::out(3, "Successfully Loaded File!");
}

void AudioRegion::schedule() {
    float timeEnd = length + gridLocation;

    {
        ContextRenderLock r(track->getAudioContext().get(), "Horizon");
        audioClipNode->reset(r);
    }
    audioClipNode->initialize();

    if (track->getAudioManager()->getCurrentGridTime() > gridLocation && track->getAudioManager()->getCurrentGridTime() < timeEnd) {

        debug::out(3, "Scheduled region during playhead");
        float playheadDiff = track->getAudioManager()->getCurrentGridTime() - gridLocation;
        audioClipNode->startGrain(context->currentTime(), track->getAudioManager()->gridTimeToSeconds(playheadDiff));
        return;
    }

    if (track->getAudioManager()->getCurrentGridTime() <= gridLocation ) {
        debug::out(3, "Scheduled region ahead of playhead");
        double timeToGo = context->currentTime() + (track->getAudioManager()->gridTimeToSeconds(gridLocation - track->getAudioManager()->getCurrentGridTime()));
        audioClipNode->start(timeToGo);
        return;
    }
}

void AudioRegion::cancelSchedule() {

    audioClipNode->stop(context->currentTime());
    {
        ContextRenderLock r(track->getAudioContext().get(), "Horizon");
        audioClipNode->reset(r);
    }


}

void AudioRegion::disconnectTrack() {
    cancelSchedule();
    debug::out(3, "Audio Region Disconnect Called --------------");
    Region::disconnectTrack();
}

void AudioRegion::setTrack(Track *_track) {

    schedule();
    Region::setTrack(_track);
}
