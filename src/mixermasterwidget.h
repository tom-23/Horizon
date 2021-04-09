#ifndef MIXERMASTERWIDGET_H
#define MIXERMASTERWIDGET_H

#include <QWidget>
#include "app/audiomanager.h"
#include "gui/meterwidget.h"
#include "gui/ui_controls/horizonmeter.h"

namespace Ui {
class MixerMasterWidget;
}

class MixerMasterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MixerMasterWidget(QWidget *parent = nullptr, Track *_track = nullptr);
    ~MixerMasterWidget();

    void uiUpdate();

private slots:
    void on_gainSlider_valueChanged(int value);

    void on_muteButton_toggled(bool checked);

    void on_panSlider_valueChanged(int value);

private:
    Ui::MixerMasterWidget *ui;

    Track *track;

    bool shiftDown;

    MeterWidget *mtr;
    QTimer *uiTimer;

    int lastLRMS = 0;
    int lastRRMS = 0;

    int uiLRMS = 0;
    int uiRRMS = 0;

    int uiLPWR = 0;
    int uiRPWR= 0;

    int updateTimeout;

    float lastPan;
    float lastGain;


};

#endif // MIXERMASTERWIDGET_H
