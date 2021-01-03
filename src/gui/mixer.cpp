#include "mixer.h"

Mixer::Mixer(QScrollArea *_mixerWidget)
{
    mixerChannelList = {};
    mixerWidget = _mixerWidget;
}

void Mixer::addChannel(Track *_track) {

    MixerChannelWidget* mcw = new MixerChannelWidget(nullptr, _track);
    _track->setMixerChannelWidget(mcw);
    mcw->setHZoomFactor(hZoomFactor);
    mixerChannelList.append(mcw);

    QHBoxLayout* mixerChannelsLayout = qobject_cast<QHBoxLayout*>(mixerWidget->widget()->layout());
    mixerChannelsLayout->insertWidget(mixerChannelsLayout->count() - 1, mcw);

}

void Mixer::clearAll() {
    mixerChannelList.clear();
}


void Mixer::setHZoomFactor(int value) {
    hZoomFactor = value;
    for (int i = 0; i < mixerChannelList.size(); i++) {
        mixerChannelList.at(i)->setHZoomFactor(value);
    }
}
