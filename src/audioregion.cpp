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

    length = track->getAudioManager()->contextSecondsToGridTime(audioClipNode->duration()) - 1;
    debug::out(3, "Length calculated");
    regionGraphicsItem->setGridLength(length);

    debug::out(3, "Successfully Loaded File!");

}

void AudioRegion::schedule() {
    float timeEnd = length + gridLocation;
    if (track->getAudioManager()->getCurrentGridTime() > timeEnd) {
        // AFTER
        debug::out(3, "AFTER");
    }
    if (track->getAudioManager()->getCurrentGridTime() > gridLocation && track->getAudioManager()->getCurrentGridTime() < timeEnd) {
        // DURING
        debug::out(3, "DURING");
        float differenceBar = track->getAudioManager()->getCurrentGridTime() - (gridLocation - 1.0);
        //qDebug() << "Current Grid Loc" << track->getAudioManager()->getCurrentGridTime();
        //qDebug() << gridLocation;
        //qDebug() << "Difference" << differenceBar;
        //qDebug() << "Seconds Differnce" << track->getAudioManager()->gridTimeToSeconds(differenceBar + 1.0);
        //qDebug() << "Duration" << audioClipNode->duration();
        audioClipNode->startGrain(context->currentTime(), track->getAudioManager()->gridTimeToSeconds(differenceBar + 1.0));

    }

    if (track->getAudioManager()->getCurrentGridTime() < gridLocation) {
        // BEFORE
        debug::out(3, "BEFORE");
        audioClipNode->start(track->getAudioManager()->gridTimeToContextSeconds(gridLocation ));
    }
}

void AudioRegion::cancelSchedule() {
    audioClipNode->stop(context->currentTime());
    {
        ContextRenderLock r(track->getAudioContext().get(), "Horizon");
        audioClipNode->reset(r);
    }


}
