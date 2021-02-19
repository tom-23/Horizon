#include "track.h"

Track::Track(Timeline &_timeLine, AudioManager &_audioMan, std::string _uuid) {

    debug::out(3, "Creating track");
    audioMan = &_audioMan;
    lab::AudioContext& ac = *audioMan->context.get();

    debug::out(3, "setting timeline");
    timeline = &_timeLine;
    debug::out(3, "setting input node");

    trackInputNode = std::make_shared<GainNode>(ac);
    debug::out(3, "setting output node");

    uuid = _uuid;

    trackOutputNode = std::make_shared<GainNode>(ac);
    pannerNode = std::make_shared<StereoPannerNode>(ac);
    Lanalyser = std::make_shared<AnalyserNode>(ac);
    Ranalyser = std::make_shared<AnalyserNode>(ac);

     Lanalyser->setSmoothingTimeConstant(8.0);
     Ranalyser->setSmoothingTimeConstant(8.0);

    channelSplitter = std::make_shared<ChannelSplitterNode>(ac, 2);
    channelMerger = std::make_shared<ChannelMergerNode>(ac, 2);


    trackInputNode->gain()->setValue(1.0f);
    trackOutputNode->gain()->setValue(1.0f);

    audioMan->context.get()->connect(trackOutputNode, trackInputNode);
    audioMan->context.get()->connect(pannerNode, trackOutputNode);
    audioMan->context.get()->connect(channelSplitter, pannerNode);


    audioMan->context.get()->connect(Lanalyser, channelSplitter, 0, 0);
    audioMan->context.get()->connect(Ranalyser, channelSplitter, 0, 1);

    audioMan->context.get()->connect(channelMerger, Lanalyser, 0, 0);
    audioMan->context.get()->connect(channelMerger, Ranalyser, 1, 0);


    audioMan->context->connect(audioMan->getOutputNode(), channelMerger);



    selected = false;
    regionList = new std::vector<class Region *>;
    selectedRegionList = new std::vector<class Region *>;

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

AudioRegion* Track::addAudioRegion(std::string regionUUID) {

    AudioRegion *audioRegion = new AudioRegion(timeline, this, regionUUID);
    regionList->push_back(audioRegion);
    return audioRegion;
}

void Track::setRegion(Region *_region) {

    regionList->insert(regionList->end(), _region);
}

void Track::removeRegion(Region *_region) {

    regionList->erase(regionList->begin() + getIndexByRegion(_region));
    trackInputNode->uninitialize();

    Lanalyser->uninitialize();
    Ranalyser->uninitialize();

    audioMan->context->disconnect(trackInputNode, _region->getOutputNode());
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

    float sumOfSquares = 0.0;
    float peakInstantaneousPower = 0.0;

    for (int i = 0; i < (int)buffer.size(); i++) {
        sumOfSquares += pow(buffer[i], 2);
        float power = pow(buffer[i], 2);
        peakInstantaneousPower = max(power, peakInstantaneousPower);
    }
    float avgPowerDecibels = 10 * log10(sumOfSquares / buffer.size());
    float peakInstantaneousPowerDecibels = 10 * log10(peakInstantaneousPower);

    if (avgPowerDecibels >= peakdB) {
        peakdB = std::ceil(avgPowerDecibels * 100.0) / 100.0;

    }
    return std::vector<int> {static_cast<int>(round(avgPowerDecibels)), static_cast<int>(round(peakInstantaneousPowerDecibels))};

}

std::vector<int> Track::getRMeterData() {

    std::vector<float> buffer(2048);

    Ranalyser->getFloatTimeDomainData(buffer);

    float sumOfSquares = 0.0;
    float peakInstantaneousPower = 0.0;

    for (int i = 0; i < (int)buffer.size(); i++) {
        sumOfSquares += pow(buffer[i], 2);
        float power = pow(buffer[i], 2);
        peakInstantaneousPower = max(power, peakInstantaneousPower);
    }
    float avgPowerDecibels = 10 * log10(sumOfSquares / buffer.size());
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

Region* Track::getSelectedRegion(int index) {
    if (selectedRegionList->size() != 0) {
        return selectedRegionList->at(index);
    } else {
        return nullptr;
    }
}

void Track::setRegionSelected(Region *region, bool selected) {
    if (selected == true) {
        debug::out(3, "Pushing region to vector...");
        for(int t = 0; t < audioMan->getTrackListCount(); t++) {
            Track *track = audioMan->getTrackByIndex(t);
            for (int i = 0; i < int(track->selectedRegionList->size()); i++ ) {
                track->setRegionSelected(track->selectedRegionList->at(i), false);
            }
        }

        selectedRegionList->clear();
        selectedRegionList->push_back(region);
        debug::out(3, "Setting region as selected...");
        region->setSelected(true);
    } else {
        auto iterator = std::find(selectedRegionList->begin(), selectedRegionList->end(), region);

        if (iterator != selectedRegionList->end()) {
            int index = std::distance(selectedRegionList->begin(), iterator);
            selectedRegionList->erase(selectedRegionList->begin() + index);
            region->setSelected(false);
        }
    }
}

AudioEffect* Track::addAudioEffect(effectType type, std::string uuid) {
    // this has very little reason to be here. It will probably get nuked one day but it's not doing harm at the moment.
    if (uuid == "") {
        uuid = "testUUID";
    }

    if (type == effectType::compressor) {
        CompressorEffect *compressorEffect = new CompressorEffect(mixerChannelWidget);
        audioEffectChain.push_back(compressorEffect);
        compressorEffect->showEffectWindow();
        return compressorEffect;
    }
    return nullptr;
}

bool Track::isLSilent() {
    {
        ContextRenderLock r(audioMan->context.get(), "Horizon");
        return Lanalyser->inputsAreSilent(r);
    }
}

bool Track::isRSilent() {
    {
        ContextRenderLock r(audioMan->context.get(), "Horizon");
        return Ranalyser->inputsAreSilent(r);
    }
}