#ifndef COMPRESSOREFFECT_H
#define COMPRESSOREFFECT_H

#include "app/audioeffect.h"
#include "gui/audio_effects/compressoreffectwindow.h"

class CompressorEffect : public AudioEffect
{
public:
    CompressorEffect(QWidget *parent);
};

#endif // COMPRESSOREFFECT_H
