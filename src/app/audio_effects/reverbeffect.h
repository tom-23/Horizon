#ifndef REVERBEFFECT_H
#define REVERBEFFECT_H

#include "app/audioeffect.h"
#include "gui/audio_effects/reverbfxwindow.h"

class ReverbEffect : public AudioEffect
{
public:
    ReverbEffect(QWidget *parent);
};

#endif // REVERBEFFECT_H
