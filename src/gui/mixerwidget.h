#ifndef MIXERWIDGET_H
#define MIXERWIDGET_H

#include <QWidget>

#include "mixer.h"

namespace Ui {
class MixerWidget;
}

class MixerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MixerWidget(QWidget *parent = nullptr);
    ~MixerWidget();

    Mixer *mixer;

private slots:
    void on_zoomSlider_valueChanged(int value);

private:
    Ui::MixerWidget *ui;
};

#endif // MIXERWIDGET_H
