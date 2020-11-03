#include "mixer.h"

Mixer::Mixer(QScrollArea *_mixerWidget)
{
    mixerWidget = _mixerWidget;
}

void Mixer::addChannel(Track *_track) {

    MixerChannelWidget* mcw = new MixerChannelWidget(nullptr, _track);
    _track->setMixerChannelWidget(mcw);

    QHBoxLayout* mixerChannelsLayout = qobject_cast<QHBoxLayout*>(mixerWidget->widget()->layout());
    mixerChannelsLayout->insertWidget(mixerChannelsLayout->count() - 1, mcw);

}

void Mixer::clearAll() {
    QLayoutItem *child;
    while ((child = mixerWidget->widget()->layout()->takeAt(0)) != 0) {
      delete child->widget();
      delete child;
    }
}
