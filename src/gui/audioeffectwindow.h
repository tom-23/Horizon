#ifndef AUDIOEFFECTWINDOW_H
#define AUDIOEFFECTWINDOW_H

#include <QDialog>

class AudioEffect;

namespace Ui {
class AudioEffectWindow;
}

class AudioEffectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AudioEffectWindow(QWidget *parent = nullptr, AudioEffect *_audioEffect = nullptr);
    ~AudioEffectWindow();

    void setEffectUI(QWidget *uiWidget);


private slots:
    void on_enabledButton_toggled(bool checked);

private:
    Ui::AudioEffectWindow *ui;
    AudioEffect *audioEffect;
};

#endif // AUDIOEFFECTWINDOW_H
