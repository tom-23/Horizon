#ifndef TRACKCONTROLSWIDGET_H
#define TRACKCONTROLSWIDGET_H

#include <QWidget>
#include "audiomanager.h"
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


private:
    Ui::TrackControlsWidget *ui;
    Track *track;

    bool shiftDown;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

};

#endif // TRACKCONTROLSWIDGET_H
