#ifndef MIXER_H
#define MIXER_H

#include <QScrollArea>
#include <QHBoxLayout>

class MixerChannelWidget;
#include "mixerchannelwidget.h"

class AudioManager;
class Track;


class Mixer
{
public:
    Mixer(QScrollArea *_mixerWidget);
    void addChannel(Track *_track);
    void clearAll();
private:
    QScrollArea *mixerWidget;
};

#endif // MIXER_H
