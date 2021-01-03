#include "reverbeffect.h"

ReverbEffect::ReverbEffect(QWidget *parent) : AudioEffect(parent)
{
    effectUI = new ReverbFXWindow(effectDialog);
    effectDialog->setEffectUI(effectUI);
}
