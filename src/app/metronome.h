#ifndef METRONOME_H
#define METRONOME_H

class AudioManager;

#include "audiomanager.h"

using namespace lab;


class Metronome
{
public:
    Metronome(std::shared_ptr<GainNode> _outputNode, AudioManager *audioMan);
    bool scheduled;
    void schedulePrimary(double when);
    void scheduleSecondary(std::vector<double> _scheduleQueue);

    void update();

private:
    std::shared_ptr<AudioBus> metPrimaryBus;
    std::shared_ptr<SampledAudioNode> metPrimaryNode;

    std::shared_ptr<AudioBus> metSecondaryBus;
    std::shared_ptr<SampledAudioNode> metSecondaryNode;


    std::shared_ptr<GainNode> outputNode;

    AudioManager *audioMan;

    std::vector<double> scheduleQueue;
    std::vector<double> scheduleQueueTimes;

    bool nextSchedule;


};

#endif // METRONOME_H
