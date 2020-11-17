#include "track.h"

Track::Track(Timeline &_timeLine, AudioManager &_audioMan, std::string _uuid) {

    debug::out(3, "Creating track");
    audioMan = &_audioMan;
    debug::out(3, "setting timeline");
    timeline = &_timeLine;
    debug::out(3, "setting input node");
    trackInputNode = std::make_shared<GainNode>();
    debug::out(3, "setting output node");

    uuid = _uuid;

    trackOutputNode = std::make_shared<GainNode>();
    pannerNode = std::make_shared<StereoPannerNode>();
    Lanalyser = std::make_shared<AnalyserNode>();
    Ranalyser = std::make_shared<AnalyserNode>();

     Lanalyser->setSmoothingTimeConstant(0.0);
     Ranalyser->setSmoothingTimeConstant(0.0);

    channelSplitter = std::make_shared<ChannelSplitterNode>(2);
    channelMerger = std::make_shared<ChannelMergerNode>(2);


    trackInputNode->gain()->setValue(1.0f);
    trackOutputNode->gain()->setValue(1.0f);

    audioMan->context.get()->connect(trackOutputNode, trackInputNode);
    audioMan->context.get()->connect(pannerNode, trackOutputNode);
    audioMan->context.get()->connect(channelSplitter, pannerNode);

    //channelSplitter->addOutputs(2);

    audioMan->context.get()->connect(Lanalyser, channelSplitter, 0, 0);
    audioMan->context.get()->connect(Ranalyser, channelSplitter, 0, 1);

    audioMan->context.get()->connect(channelMerger, Lanalyser, 0, 0);
    audioMan->context.get()->connect(channelMerger, Ranalyser, 1, 0);


    audioMan->context->connect(audioMan->getOutputNode(), channelMerger);



    selected = false;
    regionList = new std::vector<class Region *>;


    //setGain(0.0f);
   // setPan(0.0f);
    //gain = 1.0f;
    peakdB = -100;
    setMute(false);
    setGain(0.0);
    setPan(0.0);

}



Track::~Track() {

    for (auto r : *regionList) {
        delete r;
    }
    delete trackControlWidget;
    delete mixerChannelWidget;
    delete trackGraphicItem;

    audioMan->context->disconnect(audioMan->getOutputNode(), trackOutputNode);
    audioMan->context->disconnect(trackInputNode, trackOutputNode);
    qDebug() << "distroying";
}

void Track::setTrackControlsWidget(TrackControlsWidget *_tcw) {
    trackControlWidget = _tcw;

}

void Track::setMixerChannelWidget(MixerChannelWidget *_mcw) {
    mixerChannelWidget = _mcw;
}

void Track::setTrackGraphicsItem(TrackGraphicItem *_tgi) {
    trackGraphicItem = _tgi;
}

void Track::setHScaleFactor(int _hScaleFactor) {

}

AudioRegion* Track::addAudioRegion(std::string regionUUID) {

    AudioRegion *audioRegion = new AudioRegion(timeline, this, regionUUID);
    regionList->push_back(audioRegion);
    return audioRegion;
}

void Track::setRegion(Region *_region) {

    regionList->insert(regionList->end(), _region);
}

void Track::removeRegion(Region *_region) {

    qDebug() << "Removing Region... IDX" << index;
    regionList->erase(regionList->begin() + getIndexByRegion(_region));
    trackInputNode->uninitialize();

    Lanalyser->uninitialize();
    Ranalyser->uninitialize();

    qDebug() << "Track connections before:" << trackInputNode->numberOfInputs();
    audioMan->context->disconnect(trackInputNode, _region->getOutputNode());

    qDebug() << "Track connections after:" << trackInputNode->numberOfInputs();
    trackInputNode->initialize();

    Lanalyser->initialize();
    Ranalyser->initialize();
}

AudioManager* Track::getAudioManager() {
    return audioMan;
}

int Track::getIndex() {
    return index;
}

void Track::setIndex(int _index) {
    index = _index;
}

void Track::setSelected(bool _selected) {
    selected = _selected;
    trackControlWidget->setSelected(selected);
    mixerChannelWidget->setSelected(selected);
}

bool Track::getSelected() {
    return selected;
}

int Track::getIndexByRegion(Region *region) {
    auto iterator = std::find(regionList->begin(), regionList->end(), region);

    if (iterator != regionList->end()) {
       return std::distance(regionList->begin(), iterator);
    } else {
        return -1;
    }

}

std::shared_ptr<GainNode> Track::getTrackInputNode() {
    return trackInputNode;
}

std::shared_ptr<GainNode> Track::getTrackOutputNode() {
    return trackOutputNode;
}

void Track::scheduleAudioRegions() {
    for (int i = 0; i < int(regionList->size()); i++) {
        AudioRegion* audioRegion = dynamic_cast<AudioRegion*>(regionList->at(i));
        //double contextLocation = audioMan->gridTimeToContextSeconds(audioRegion->getGridLocation()) - audioMan->getCurrentRelativeTime();
        audioRegion->schedule();
        debug::out(3, "Scheduled a region...");
    }
}

void Track::cancelAudioRegions() {
    for (int i = 0; i < int(regionList->size()); i++) {
        AudioRegion* audioRegion = dynamic_cast<AudioRegion*>(regionList->at(i));
        audioRegion->cancelSchedule();
        debug::out(3, "Cancelling a region...");
    }
}

//void Track::removeRegion(int position) {
//    std::vector<class Region *>::iterator it = std::find(regionList->begin(), regionList->end(), _region);
//    if (it != regionList->end()) {
//
//        regionList->erase(std::distance(regionList->begin(), it));
//    }
//}

void Track::setGain(float _value) {
    gain = pow(10, (_value / 20));
    gainNonLog = _value;
    qDebug() << "Setting Gain" << gain;
    if (!mute) {
        trackOutputNode->gain()->setValue(gain);
    }

}

float Track::getGain() {

    return gainNonLog;
}

void Track::setPan(float _value) {
    pan = _value;
    pannerNode->pan()->setValue(_value);
}

float Track::getPan() {
    return pan;
}

void Track::setMute(bool _mute) {
    mute = _mute;
    if (mute == true) {
        trackOutputNode->gain()->setValue(0.0f);
    } else {
        trackOutputNode->gain()->setValue(gain);
    }
}

void Track::setSolo(bool _solo) {
    solo = _solo;
    if (solo == true) {

    }
}

bool Track::getMute() {
    return mute;
}

bool Track::getSolo() {
    return solo;
}

QColor Track::getColor() {
    return color;
}

void Track::setColor(QColor _color) {
    color = _color;
}

void Track::updateColor(QColor _color) {
    color = _color;
    trackControlWidget->updateColor();
    mixerChannelWidget->updateColor();
}

std::vector<int> Track::getLMeterData() {

    std::vector<float> buffer(2048);

    Lanalyser->getFloatTimeDomainData(buffer);

    //analyser->getFloatFrequencyData(buffer);
   // qDebug() << "BUFFER" << buffer[0];

    float sumOfSquares = 0;
    for (int i = 0; i < (int)buffer.size(); i++) {
        sumOfSquares += pow(buffer[i], 2);

    }
    //qDebug() << "SOS" << sumOfSquares;

    float avgPowerDecibels = 10 * log10(sumOfSquares / buffer.size());
    int peakInstantaneousPower = 0;

    for (int i = 0; i < (int)buffer.size(); i++) {
          int power = pow(buffer[i], 2);
          peakInstantaneousPower = max(power, peakInstantaneousPower);
    }

    float peakInstantaneousPowerDecibels = 10 * log10(peakInstantaneousPower);

    if (avgPowerDecibels >= peakdB) {
        peakdB = std::ceil(avgPowerDecibels * 100.0) / 100.0;

    }



    return std::vector<int> {static_cast<int>(round(avgPowerDecibels)), static_cast<int>(round(peakInstantaneousPowerDecibels))};

}

std::vector<int> Track::getRMeterData() {

    std::vector<float> buffer(2048);

    Ranalyser->getFloatTimeDomainData(buffer);

    //analyser->getFloatFrequencyData(buffer);
   // qDebug() << "BUFFER" << buffer[0];

    float sumOfSquares = 0;
    for (int i = 0; i < (int)buffer.size(); i++) {
        sumOfSquares += pow(buffer[i], 2);

    }
    //qDebug() << "SOS" << sumOfSquares;

    float avgPowerDecibels = 10 * log10(sumOfSquares / buffer.size());
    int peakInstantaneousPower = 0;

    for (int i = 0; i < (int)buffer.size(); i++) {
          int power = pow(buffer[i], 2);
          peakInstantaneousPower = max(power, peakInstantaneousPower);
    }

    float peakInstantaneousPowerDecibels = 10 * log10(peakInstantaneousPower);

    if (avgPowerDecibels >= peakdB) {
        peakdB = std::ceil(avgPowerDecibels * 100.0) / 100.0;

    }
    return std::vector<int> {static_cast<int>(round(avgPowerDecibels)), static_cast<int>(round(peakInstantaneousPowerDecibels))};

}


int Track::getAudioRegionListCount() {
    return regionList->size();
}

AudioRegion* Track::getAudioRegionByIndex(int index) {
    return dynamic_cast<AudioRegion*>(regionList->at(index));
}

std::string Track::getUUID() {
    return uuid;
}

void Track::uiUpdate() {
    trackControlWidget->uiUpdate();
    mixerChannelWidget->uiUpdate();
}
