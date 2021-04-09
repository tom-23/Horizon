#include "audioschedulingthread.h"

Q_DECLARE_METATYPE(std::vector<Track *>);

AudioSchedulingThread::AudioSchedulingThread()
{

}

void AudioSchedulingThread::run() {
    //connect(updateTimer, &QTimer::timeout, this, [=] () {
    std::thread threadObj([=] () {
      //  while (true) {
        //for (int i = 0; i < int(trackList->size()); i++) {
        //    if (trackList->at(i)->getAudioManager()->isPlaying) {
        //        trackList->at(i)->scheduleNextBar();
        //    }
        //}
        //thread()->sleep(5);
      //  }

    });
    //});
    //updateTimer->setInterval(1);
    //updateTimer->start();
}
