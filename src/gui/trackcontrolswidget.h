#ifndef TRACKCONTROLSWIDGET_H
#define TRACKCONTROLSWIDGET_H

#include <QWidget>
#include "app/audiomanager.h"
#include "gui/colorpickerwidget.h"
#include "gui/meterwidget.h"
class Track;
class Timeline;

namespace Ui {
class TrackControlsWidget;
}

class TrackControlsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrackControlsWidget(QWidget *parent = nullptr, Track *_track = nullptr);
    ~TrackControlsWidget();

    void setSelected(bool selected);

private slots:


    void on_TrackControlsWidget_customContextMenuRequested(const QPoint &pos);


    void on_muteButton_toggled(bool checked);

    void on_armButton_clicked();

    void on_peakdBLabel_clicked();

private:
    Ui::TrackControlsWidget *ui;
    Track *track;

    bool shiftDown;

    QTimer *uiTimer;
    void uiUpdate();

    int lastMeterValue;

    int currentMeterValue;

    MeterWidget *mtr;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void changeColor();

};

#endif // TRACKCONTROLSWIDGET_H
