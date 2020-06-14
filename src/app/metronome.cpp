#include "metronome.h"

Metronome::Metronome(std::shared_ptr<GainNode> _outputNode, AudioManager *_audioMan)
{
    outputNode = _outputNode;
    audioMan = _audioMan;

    metPrimaryBus = audioMan->MakeBusFromSampleFile(util::getResourceBundle() + "/core/metronome/Primary.wav");
    metSecondaryBus = audioMan->MakeBusFromSampleFile(util::getResourceBundle() + "/core/metronome/Secondary.wav");

    metPrimaryNode = std::make_shared<SampledAudioNode>();
    {
        ContextRenderLock r(audioMan->context.get(), "horizon");
        metPrimaryNode->setBus(r, metPrimaryBus);
    }

    metSecondaryNode = std::make_shared<SampledAudioNode>();
    {
        ContextRenderLock r(audioMan->context.get(), "horizon");
        metSecondaryNode->setBus(r, metSecondaryBus);
    }

    audioMan->context->connect(outputNode, metPrimaryNode);
    audioMan->context->connect(outputNode, metSecondaryNode);
}


void Metronome::schedulePrimary(double when) {
    //metPrimaryNode->start(audioMan->gridTimeToContextSeconds(when));
}

void Metronome::scheduleSecondary(std::vector<double> _scheduleQueue) {
    scheduleQueue = _scheduleQueue;
    scheduleQueueTimes.clear();

    for (std::vector<double>::size_type i = 0; i != scheduleQueue.size(); i++) {
        scheduleQueueTimes.insert(scheduleQueueTimes.end(), audioMan->gridTimeToContextSeconds(scheduleQueue.at(i)));
    }
    //qDebug() << scheduleQueueTimes;


}

void Metronome::update() {
    if (scheduleQueueTimes.size() != 0) {
        double timeTillClick = scheduleQueueTimes.at(0) - audioMan->context->currentTime();
        if (timeTillClick < 0.1) {
            if (nextSchedule == true) {
                double time = scheduleQueueTimes.at(0);
                //metSecondaryNode->start(time);
                scheduleQueueTimes.erase(scheduleQueueTimes.begin());
                nextSchedule = false;
            }
        } else {
            nextSchedule = true;
        }
        //qDebug() << timeTillClick;
    }
}
