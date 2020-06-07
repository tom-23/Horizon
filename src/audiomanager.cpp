#include "audiomanager.h"
#include "debug.h"

AudioManager::AudioManager()
{

    stopTime = 0.0;
    currentGridTime = 1.0;
    debug::out(3, "Starting audio engine...");

    const auto defaultAudioDeviceConfigurations = GetDefaultAudioDeviceConfiguration();
    context = lab::MakeRealtimeAudioContext(defaultAudioDeviceConfigurations.second, defaultAudioDeviceConfigurations.first);

    outputNode = std::make_shared<GainNode>();
    outputNode->gain()->setValue(1.0f);

    context->connect(context->device(), outputNode);

    TrackAudio *tn = new TrackAudio(context, outputNode, 0);
    AudioRegionManager *arm = new AudioRegionManager(context, tn->getTrackNode(), 0, 0);

    debug::out(3, "Loading metronome samples...");

    metPrimaryBus = MakeBusFromSampleFile("../Resources/core/metronome/Primary.wav");
    metPrimaryBus = MakeBusFromSampleFile("../Resources/core/metronome/Secondary.wav");


    metPrimaryNode = std::make_shared<SampledAudioNode>();
    {
        ContextRenderLock r(context.get(), "met_primary");
        metPrimaryNode->setBus(r, metPrimaryBus);
    }

    context->connect(outputNode, metPrimaryNode);


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

void AudioManager::updateSchedule() {

}

void AudioManager::eventLoop() {
    float relativeTime = (context->currentTime() - startTime) + stopTime;
    currentGridTime = ((relativeTime / beatLength) / division) + 1.0;
    //qDebug() << "CURRENT TIME" << relativeTime;
    //qDebug() << "GRID TIME" << currentGridTime;
    if ((floor(currentGridTime) - 0.8) < currentGridTime || (floor(currentGridTime) - 0.8) > currentGridTime) {
        //metPrimaryNode->start(floor(currentGridTime));
        //debug::out(3, "Buffered Primary Met");
    }
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
