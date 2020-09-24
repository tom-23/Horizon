#include "audiomanager.h"


AudioManager::AudioManager(Timeline &_timeline)
{
    debug::out(3, "Timeline init");
    timeline = &_timeline;
    stopTime = 0.0;
    isPlaying = false;
    currentGridTime = 1.0;
    scheduled = false;
    debug::out(3, "Starting audio engine...");

    const auto defaultAudioDeviceConfigurations = GetDefaultAudioDeviceConfiguration();
    context = lab::MakeRealtimeAudioContext(defaultAudioDeviceConfigurations.second, defaultAudioDeviceConfigurations.first);

    outputNode = std::make_shared<GainNode>();
    outputNode->gain()->setValue(1.0f);
    context->connect(context->device(), outputNode);

    trackList = new std::vector<class Track *>();
    selectedTrackList = new std::vector<class Track *>();

    debug::out(3, "Loading metronome...");

    metronome = new Metronome(outputNode, this);


    debug::out(3, "Starting event loop...");
    eventTimer = new Timer(std::bind(&AudioManager::eventLoop, this)), std::chrono::milliseconds(5);
    eventTimer->setSingleShot(false);


    debug::out(3, "Audio engine started without any issues!");
}

std::shared_ptr<AudioBus> AudioManager::MakeBusFromSampleFile(std::string fileName) {

        std::shared_ptr<AudioBus> bus = MakeBusFromFile(fileName, false);
        if (!bus) {
            debug::out(1, "COULD NOT OPEN FILE: " + fileName);
            return nullptr;
        }
        return bus;
}


void AudioManager::play() {
    if (isPlaying == false) {
        startTime = context->currentTime();
        updateMetSchedule();
        scheduleTracks();
        isPlaying = true;
        eventTimer->start(true);
    }
}

void AudioManager::pause() {
    if (isPlaying == true) {
        isPlaying = false;
        cancelTrackPlayback();
        eventTimer->stop();
        stopTime = getCurrentRelativeTime();
    }
}


void AudioManager::stop() {

    if (isPlaying == true) {
        isPlaying = false;
        cancelTrackPlayback();
        eventTimer->stop();
    }
    stopTime = 0.0;
    currentGridTime = 1.0;
}

void AudioManager::setLookAhead(double _value) {
    lookAhead = _value;
}

void AudioManager::updateMetSchedule() {
    metronome->schedulePrimary(floor(currentGridTime) + 1);
    double divGrid = 1.00 / division;
    std::vector<double> scheduleQueue;
    for (int i = 1; i < division; i++) {
        scheduleQueue.insert(scheduleQueue.end(), (floor(currentGridTime) + 1) + (i * divGrid));

    }

    metronome->scheduleSecondary(scheduleQueue);

}
void AudioManager::updateSchedule() {

    double toNearestBar = (floor(currentGridTime) + 1) - currentGridTime;
    if (toNearestBar < lookAhead || currentGridTime == 0) {
        //metPrimaryNode->start(floor(currentGridTime));
        //debug::out(3, "Buffered Primary Met");
        if (toNearestBar < 0.01) {

            if (scheduled == true) {

                scheduled = false;
            }

        } else {
            if (scheduled == false) {
                updateMetSchedule();
                debug::out(3, "Scheduling...");
                scheduled = true;
            }
        }

    }


    metronome->update();

}

void AudioManager::eventLoop() {
    float relativeTime = (context->currentTime() - startTime) + stopTime;
    currentGridTime = ((relativeTime / beatLength) / division) + 1.0;

    updateSchedule();
}

void AudioManager::setDivision(int _division) {
    division = _division;
    barLength = bpm * division;
}

void AudioManager::setBPM(double _beatsPerMinuet) {
    bpm = _beatsPerMinuet;
    beatLength = 60.00 / bpm;
    barLength = bpm * division;
}

float AudioManager::getCurrentGridTime() {
    return currentGridTime;
}

double AudioManager::gridTimeToContextSeconds(float _gridTime) {
    double secondsTime = ((_gridTime - 1.0) * beatLength) * division;
    return startTime + secondsTime;
}

double AudioManager::gridTimeToSeconds(float _gridTime) {
    double secondsTime = ((_gridTime) * beatLength) * division;
    return secondsTime;
}

float AudioManager::secondsToGridTime(double _seconds) {
    double gridTime = ((_seconds / beatLength) / division) + 1.0;
    return gridTime;
}

float AudioManager::getCurrentRelativeTime() {
    float relativeTime = (context->currentTime() - startTime) + stopTime;
    return relativeTime;
}

Track* AudioManager::addTrack() {
    debug::out(3, "Creating new track...");
    Track *newTrack = new Track(*timeline, *this);
    debug::out(3, "Pushing to list...");
    trackList->push_back(newTrack);
    debug::out(3, "Setting index");
    newTrack->setIndex(trackList->size() - 1);

    debug::out(3, "Dispatching to UI...");
    return newTrack;
}

Track* AudioManager::getTrackByIndex(int index) {
    return trackList->at(index);
}

Track* AudioManager::getSelectedTrack(int index) {
    if (selectedTrackList->size() != 0) {
        return selectedTrackList->at(index);
    } else {
        return nullptr;
    }

}

std::vector<class Track*>* AudioManager::getSelectedTracks() {
    return selectedTrackList;
}

std::shared_ptr<GainNode> AudioManager::getOutputNode() {
    return outputNode;
}

void AudioManager::setTrackSelected(Track *track, bool selected) {
    if (selected == true) {
        debug::out(3, "Pushing track to vector...");
        for (int i = 0; i < int(selectedTrackList->size()); i++ ) {
            setTrackSelected(selectedTrackList->at(i), false);
        }
        selectedTrackList->clear();
        selectedTrackList->push_back(track);
        debug::out(3, "Setting as selected...");
        track->setSelected(true);
    } else {
        auto iterator = std::find(selectedTrackList->begin(), selectedTrackList->end(), track);

        if (iterator != selectedTrackList->end()) {
            int index = std::distance(selectedTrackList->begin(), iterator);
            selectedTrackList->erase(selectedTrackList->begin() + index);
            track->setSelected(false);
        }
    }
}

void AudioManager::setTrackRangeSelected(Track *firstTrack, Track *lastTrack) {

    for (int i = 0; i < int(selectedTrackList->size()); i++ ) {
        setTrackSelected(selectedTrackList->at(i), false);
    }
    selectedTrackList->clear();

    auto firstIterator = std::find(trackList->begin(), trackList->end(), firstTrack);
    auto lastIterator = std::find(trackList->begin(), trackList->end(), lastTrack);

    int firstIndex;
    int lastIndex;

    if (firstIterator != trackList->end()) {
        firstIndex = std::distance(trackList->begin(), firstIterator);
    } else {
        return;
    }

    if (lastIterator != trackList->end()) {
        lastIndex = std::distance(trackList->begin(), lastIterator);
    } else {
        return;
    }

    for (int i = firstIndex; i < lastIndex; i++) {
        selectedTrackList->push_back(trackList->at(i));
        trackList->at(i)->setSelected(true);
    }
}

int AudioManager::getTrackListCount() {
    return trackList->size();
}

void AudioManager::scheduleTracks() {
    for (int i = 0; i < int(trackList->size()); i++) {
        trackList->at(i)->scheduleAudioRegions();
        debug::out(3, "Scheduled a track...");
    }
}

void AudioManager::cancelTrackPlayback() {
    for (int i = 0; i < int(trackList->size()); i++) {
        trackList->at(i)->cancelAudioRegions();
        debug::out(3, "Cancelling track...");
    }
}

void AudioManager::setCurrentGridTime(float _value) {
    currentGridTime = _value;
}

std::vector<std::vector<float>> AudioManager::calculatePeaks(std::shared_ptr<AudioBus> bus, std::string fileName) {

    std::vector<std::vector<float>> calculatedStereoSamples;


    for (int channelIdx = 0; channelIdx < (int)bus->numberOfChannels(); channelIdx++) {

         std::vector<float> channelData;

         for (int dp = 0; dp < (int)bus->channel(channelIdx)->length(); dp++) {
             channelData.push_back(log10(bus->channel(channelIdx)->data()[dp]));
         }
         calculatedStereoSamples.push_back(channelData);
    }

    return calculatedStereoSamples;
}

void AudioManager::engageSolo() {
    soloEnabled = true;
    for (int i = 0; i < int(trackList->size()); i++) {
        if (trackList->at(i)->getSolo() == false) {
            trackList->at(i)->getTrackOutputNode()->gain()->setValue(0.0f);
        }

    }
}

void AudioManager::disengageSolo() {
    soloEnabled = false;
    for (int i = 0; i < int(trackList->size()); i++) {
        if (trackList->at(i)->getSolo() == false) {
            trackList->at(i)->getTrackOutputNode()->gain()->setValue(0.0f);
        }

    }
}
