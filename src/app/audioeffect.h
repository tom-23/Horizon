#ifndef AUDIOEFFECT_H
#define AUDIOEFFECT_H

#include <QDialog>

class AudioEffectWindow;
#include "gui/audioeffectwindow.h"


//using namespace lab;

class AudioEffect
{
public:
    AudioEffect(QWidget *parent);

    //virtual std::shared_ptr<GainNode> getOutputNode();
    //virtual std::shared_ptr<GainNode> getInputNode();

    virtual void showEffectWindow();
    virtual void hideEffectWindow();

    virtual std::string getFriendlyName();

protected:
    //std::shared_ptr<GainNode> outputNode;
    //std::shared_ptr<GainNode> inputNode;

    AudioEffectWindow *effectDialog;
    QWidget *effectUI;

    std::string name;
};

#endif // AUDIOEFFECT_H
