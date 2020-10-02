#include "audioregion.h"


AudioRegion::AudioRegion(Timeline *_timeline, Track *_track) : Region(_timeline, _track)
{
    debug::out(3, "Audio region added");
}

void AudioRegion::loadFile(std::string fileName) {

    debug::out(3, "Begining file loading...");

    dialogs::ProgressDialog::show(0, 0, "Loading Audio file...");


    QFileInfo fileInfo(QString::fromStdString(fileName));
    setRegionName(fileInfo.fileName().toStdString());

    loadedFileName = fileName;

    std::thread t(&AudioRegion::loadFileThread, this, std::bind(&AudioRegion::loadedFileCallBack, this));
    t.detach();
    debug::out(3, "Spawining thread...");

}

void AudioRegion::loadFileThread(std::function<void()> callback) {
    debug::out(3, "Spawned file handling thread");
    debug::out(3, "Starting file loading...");
    audioClipBus = track->getAudioManager()->MakeBusFromSampleFile(loadedFileName);

    audioClipNode = std::make_shared<SampledAudioNode>();
    {
        ContextRenderLock r(track->getAudioManager()->context.get(), "Horizon");
        audioClipNode->setBus(r, audioClipBus);
    }



    debug::out(3, "Getting peaks...");
    length = track->getAudioManager()->secondsToGridTime(audioClipNode->duration()) - 1;

    regionGraphicsItem->setGridLength(length);
    regionGraphicsItem->setWaveform(track->getAudioManager()->getPeaks(audioClipBus), audioClipBus->length());
    //regionGraphicsItem->generateWaveform();
    debug::out(3, "Thread Finished Bye ;)");
    callback();
}


void AudioRegion::loadedFileCallBack() {
    track->getAudioManager()->context->connect(outputNode, audioClipNode);

    debug::out(3, "Length calculated");

    regionGraphicsItem->setGhost(false);

    dialogs::ProgressDialog::close();
    timeline->updateViewports();


    debug::out(3, "Successfully Loaded File!");
}

void AudioRegion::schedule() {
    float timeEnd = length + gridLocation;

    {
        ContextRenderLock r(track->getAudioManager()->context.get(), "Horizon");
        audioClipNode->reset(r);
   }

    audioClipNode->initialize();

    audioClipNode->gain()->setValue(1.0f);

    if (track->getAudioManager()->getCurrentGridTime() > gridLocation && track->getAudioManager()->getCurrentGridTime() < timeEnd) {

        debug::out(3, "Scheduled region during playhead");
        float playheadDiff = track->getAudioManager()->getCurrentGridTime() - gridLocation;
        audioClipNode->startGrain(track->getAudioManager()->context->currentTime(), track->getAudioManager()->gridTimeToSeconds(playheadDiff));
        return;
    }

    if (track->getAudioManager()->getCurrentGridTime() <= gridLocation ) {
        debug::out(3, "Scheduled region ahead of playhead");
        double timeToGo = track->getAudioManager()->context->currentTime() + (track->getAudioManager()->gridTimeToSeconds(gridLocation - track->getAudioManager()->getCurrentGridTime()));
        audioClipNode->start(timeToGo);

        return;
    }
}

void AudioRegion::cancelSchedule() {

    audioClipNode->gain()->setValue(0.0f);

    audioClipNode->stop(track->getAudioManager()->context->currentTime());
    {
        ContextRenderLock r(track->getAudioManager()->context.get(), "Horizon");
        audioClipNode->reset(r);
    }


}

void AudioRegion::disconnectTrack() {
    cancelSchedule();
    debug::out(3, "Audio Region Disconnect Called --------------");
    Region::disconnectTrack();
}

void AudioRegion::setTrack(Track *_track) {


    {
        ContextRenderLock r(track->getAudioManager()->context.get(), "Horizon");
        audioClipNode->reset(r);

    }

    debug::out(3, "Switching Tracks...");
    //outputNode->uninitialize();

    track->getTrackInputNode()->input(0)->junctionDisconnectAllOutputs();

    _track->getAudioManager()->context->connect(_track->getTrackInputNode(), outputNode);



    audioClipNode->initialize();
    debug::out(3, "Connected to track");
    setGain(gain);

    track = _track;
}

std::string AudioRegion::getLoadedFileName() {
    return loadedFileName;
}
