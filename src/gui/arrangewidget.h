#ifndef ARRANGEWIDGET_H
#define ARRANGEWIDGET_H

#include <QWidget>
//#include "trackcontrolswidget.h"

//#include "audioregion.h"
//#include "region.h"
//#include "track.h"

#include "mainwindow.h"

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

private slots:
    void on_pushButton_4_clicked();

    void on_zoomSlider_valueChanged(int value);

    void on_soloDisableButton_clicked();

private:
    Ui::ArrangeWidget *ui;
    AudioManager *audioMan;
    Mixer *mixer;


protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // ARRANGEWIDGET_H
