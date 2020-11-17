#include "audioregion.h"


AudioRegion::AudioRegion(Timeline *_timeline, Track *_track, std::string _uuid) : Region(_timeline, _track, _uuid)
{
    debug::out(3, "Audio region added");
}

void AudioRegion::loadFile(std::string fileName, bool _progressDialog) {


    debug::out(3, "Begining file loading...");

    progressDialog = _progressDialog;
    if (progressDialog == true) {
        dialogs::ProgressDialog::show(0, 0, "Loading Audio file...");
    }



    QFileInfo fileInfo(QString::fromStdString(fileName));
    setRegionName(fileInfo.fileName().toStdString());

    loadedFileName = fileName;

    fileLoading = new FileLoading(nullptr, std::bind(&AudioRegion::loadedFileCallBack, this));
    debug::out(3, "Spawining thread...");

    fileLoading->operate(track->getAudioManager(), QString::fromStdString(loadedFileName));

}

void AudioRegion::loadedFileCallBack() {
    audioClipBus = fileLoading->bus;
    audioClipNode = fileLoading->node;

    track->getAudioManager()->context->connect(outputNode, audioClipNode);

    length = track->getAudioManager()->secondsToGridTime(audioClipNode->duration()) - 1;
    debug::out(3, "Length calculated");

    regionGraphicsItem->setGhost(false);
    regionGraphicsItem->setGridLength(length);
    regionGraphicsItem->setWaveform(audioClipBus);

    if (progressDialog == true) {
       dialogs::ProgressDialog::close();
    } else {
        if (dialogs::ProgressDialog::getValue() + 1 == dialogs::ProgressDialog::getMax()) {
            dialogs::ProgressDialog::close();
        } else {
            qDebug() << "Updating progress..." << dialogs::ProgressDialog::getValue() << dialogs::ProgressDialog::getMax();
            dialogs::ProgressDialog::updateValue(dialogs::ProgressDialog::getValue() + 1);
        }
    }


    if (length > timeline->barCount) {
        timeline->setBarAmount(ceil(length));
    }

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


void AudioRegion::switchContext(AudioContext *context) {

}
