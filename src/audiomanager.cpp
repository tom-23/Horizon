#include "audiomanager.h"


AudioManager::AudioManager(Timeline *_timeline)
{

    timeline = _timeline;
    stopTime = 0.0;
    currentGridTime = 0.0;
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

    metronome = new Metronome(context, outputNode, this);


    debug::out(3, "Starting event loop...");
    eventTimer = new Timer(std::bind(&AudioManager::eventLoop, this)), std::chrono::milliseconds(5);
    eventTimer->setSingleShot(false);


    debug::out(3, "Audio engine started without any issues!");
}

std::shared_ptr<AudioBus> AudioManager::MakeBusFromSampleFile(std::string fileName) {

        std::shared_ptr<AudioBus> bus = MakeBusFromFile(fileName, false);
        if (!bus) debug::out(3, "COULD NOT OPEN FILE: " + fileName);
        return bus;
}


void AudioManager::play() {

    startTime = context->currentTime();
    updateMetSchedule();
    eventTimer->start(true);



}

void AudioManager::pause() {
    eventTimer->stop();
    stopTime = currentGridTime;
}


void AudioManager::stop() {
    eventTimer->stop();
    stopTime = 0.0;
    currentGridTime = 0.0;
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
            //qDebug() << "CALLED";
            if (scheduled == true) {
                //qDebug() << "Boop!";
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
    //qDebug() << (floor(currentGridTime) - 0.001) + 1.0;

}

void AudioManager::eventLoop() {
    float relativeTime = (context->currentTime() - startTime) + stopTime;
    currentGridTime = ((relativeTime / beatLength) / division) + 1.0;
    //qDebug() << "CURRENT TIME" << relativeTime;
    //qDebug() << "GRID TIME" << currentGridTime;

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

Track* AudioManager::addTrack() {
    debug::out(3, "Creating new track...");
    Track *newTrack = new Track(timeline, this);
    debug::out(3, "Pushing to list...");
    trackList->push_back(newTrack);
    newTrack->setIndex(trackList->size() - 1);

    debug::out(3, "Dispatching to UI...");
    return newTrack;
}

Track* AudioManager::getTrackByIndex(int index) {
    return trackList->at(index);
}

Track* AudioManager::getSelectedTrack(int index) {
    return selectedTrackList->at(index);
}

std::vector<class Track*>* AudioManager::getSelectedTracks() {
    return selectedTrackList;
}

std::shared_ptr<AudioContext> AudioManager::getAudioContext() {
    return context;
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
