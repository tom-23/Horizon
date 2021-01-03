#include "compressoreffect.h"

CompressorEffect::CompressorEffect(QWidget *parent) : AudioEffect(parent)
{
    name = "Horizon Compressor";
    effectUI = new CompressorEffectWindow(effectDialog);
    effectDialog->setEffectUI(effectUI);
}
