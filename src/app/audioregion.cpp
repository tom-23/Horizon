#include "audioregion.h"


AudioRegion::AudioRegion(Timeline *_timeline, Track *_track, QString _uuid) : Region(_timeline, _track, _uuid)
{
    debug::out(3, "Audio region added");
    audioManager = _track->getAudioManager();
}

AudioRegion::~AudioRegion() {

}

void AudioRegion::loadFile(QString fileName, bool _progressDialog) {

    debug::out(3, "Begining file loading...");
    audioManager->sendCommand("loadAudioRegion", uuid, fileName);

    progressDialog = _progressDialog;
    if (progressDialog == true) {
        dialogs::ProgressDialog::show(0, 0, "Loading Audio file...");
    }



    QFileInfo fileInfo(fileName);
    setRegionName(fileInfo.fileName());

    loadedFileName = fileName;

    //fileLoading = new FileLoading(nullptr, std::bind(&AudioRegion::loadedFileCallBack, this));
    //debug::out(3, "Spawining thread...");

}

void AudioRegion::loadedFileCallBack(double _length) {

    length = _length;
    updateGridLength();
    debug::out(3, "Length calculated");




    if (progressDialog == true) {
       dialogs::ProgressDialog::close();
    } else {
        if (dialogs::ProgressDialog::getValue() + 1 == dialogs::ProgressDialog::getMax()) {
            dialogs::ProgressDialog::close();
        } else {
            dialogs::ProgressDialog::updateValue(dialogs::ProgressDialog::getValue() + 1);
        }
    }

    regionGraphicsItem->setGhost(false);
    regionGraphicsItem->setGridLength(length);
    //regionGraphicsItem->setWaveform(audioClipBus);

    if (length > timeline->barCount) {
        timeline->setBarAmount(ceil(length));
    }
    timeline->updateViewports();

    isScheduled = false;
    debug::out(3, "Successfully Loaded File!");
}


QString AudioRegion::getLoadedFileName() {
    return loadedFileName;
}


void AudioRegion::updateGridLength() {

}
