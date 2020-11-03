#ifndef AUDIOEFFECTWINDOW_H
#define AUDIOEFFECTWINDOW_H

#include <QDialog>

namespace Ui {
class AudioEffectWindow;
}

class AudioEffectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AudioEffectWindow(QWidget *parent = nullptr);
    ~AudioEffectWindow();

private:
    Ui::AudioEffectWindow *ui;
};

#endif // AUDIOEFFECTWINDOW_H
