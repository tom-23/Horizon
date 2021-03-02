#ifndef ARRANGEWIDGET_H
#define ARRANGEWIDGET_H

#include <QWidget>
//#include "trackcontrolswidget.h"

//#include "audioregion.h"
//#include "region.h"
//#include "track.h"

#include "mainwindow.h"
#include "ui_controls/horizonpushbutton.h"

//#include "app/audiomanager.h"

class AudioManager;
class Track;
class Timeline;
class Mixer;


namespace Ui {
class ArrangeWidget;
}

class ArrangeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ArrangeWidget(QWidget *parent);
    ~ArrangeWidget();

    Timeline *tl;

    void setAudioManager(AudioManager &_audioMan);
    void setMixer(Mixer *_mixer);
    void importAudio(QString path = "");
    Track* addAudioTrack(Track *track = nullptr, std::string uuid = "");
    void setHZoomFactor(int hZoomFactor);

    QGraphicsColorizeEffect* mEffect;

private slots:
    void on_pushButton_4_clicked();

    void on_zoomSlider_valueChanged(int value);

    void on_soloDisableButton_clicked();



    void on_centerPlayheadButton_toggled(bool checked);
    void on_hScroll_actionTriggered(int action);



    void on_pushButton_3_clicked();

    void on_pushButton_3_toggled(bool checked);

private:
    Ui::ArrangeWidget *ui;

    // keep a reference of the audio manager for use later
    AudioManager *audioMan;
    Mixer *mixer;

    // not sure if this serves a purpose.
    QPoint lastMousePos;

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // ARRANGEWIDGET_H
