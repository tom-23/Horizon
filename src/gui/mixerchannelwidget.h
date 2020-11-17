#ifndef MIXERCHANNELWIDGET_H
#define MIXERCHANNELWIDGET_H

#include <QWidget>
#include "app/audiomanager.h"
#include "gui/meterwidget.h"

namespace Ui {
class MixerChannelWidget;
}

class MixerChannelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MixerChannelWidget(QWidget *parent = nullptr, Track *_track = nullptr);
    ~MixerChannelWidget();

    void setSelected(bool selected);
    void uiUpdate();

    void updateColor();
private slots:
    void on_gainSlider_valueChanged(int value);


    void on_muteButton_toggled(bool checked);

    void on_panSlider_valueChanged(int value);

private:
    Track *track;
    Ui::MixerChannelWidget *ui;
    bool shiftDown;

    MeterWidget *mtr;
    QTimer *uiTimer;

    int lastLRMS = 0;
    int lastRRMS = 0;

    int uiLRMS = 0;
    int uiRRMS = 0;

    int updateTimeout;

    float lastPan;
    float lastGain;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // MIXERCHANNELWIDGET_H
