#include "audioeffect.h"

AudioEffect::AudioEffect(QWidget *parent)
{
    effectDialog = new AudioEffectWindow(parent);
}

std::shared_ptr<GainNode> AudioEffect::getOutputNode() {
    return outputNode;
}

std::shared_ptr<GainNode> AudioEffect::getInputNode() {
    return inputNode;
}

void AudioEffect::showEffectWindow() {
    effectDialog->exec();
}

void AudioEffect::hideEffectWindow() {
    effectDialog->hide();
}

std::string AudioEffect::getFriendlyName() {
    return name;
}
