#include <QScrollBar>
#include <QRegion>
#include <QDebug>
#include <QPainter>
#include <QFileDialog>
#include <QImage>
#include "ui_arrangewidget.h"

#include "arrangewidget.h"



ArrangeWidget::ArrangeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArrangeWidget)
{
    ui->setupUi(this);
    tl = new Timeline(this, ui->ruler, ui->trackControls, ui->timeline->layout(), ui->hScroll, ui->vScroll);
    this->repaint();
}


ArrangeWidget::~ArrangeWidget()
{
    tl->~Timeline();
    delete ui;
}

void ArrangeWidget::on_pushButton_4_clicked()
{

}


void ArrangeWidget::resizeEvent(QResizeEvent *event)
{

}


void ArrangeWidget::on_zoomSlider_valueChanged(int value)
{

    tl->setHZoomFactor(value, ui->zoomSlider);
}


void ArrangeWidget::setAudioManager(AudioManager &_audioMan) {
    audioMan = &_audioMan;
}

void ArrangeWidget::importAudio(QString path) {
    if (audioMan->getSelectedTrack(0) != nullptr) {
        if (path == "") {
            if (audioMan->isPlaying) {

                if (dialogs::MessageDialog::show("Importing audio stops playback", "This action will pause audio. Are you sure you want to continue?",
                                                 dialogs::MessageDialog::info, dialogs::MessageDialog::yesNo) == 3) {
                    return;
                } else {
                    audioMan->pause();
                }
            }
            path = QFileDialog::getOpenFileName(this,
                                                tr("Load Audio"), QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
                                                tr("Supported Audio Files (*.wav *.mp3)"));
            //QFileDialog *dialog = new QFileDialog(this);
            //dialog->setNameFilter(tr("Supported Audio Files (*.wav *.mp3 *.acc)"));
            //dialog->setModal(false);
            //path = dialog->getOpenFileName();
        }


        if (path.toStdString() != "") {
            AudioRegion *newAudioRegion = audioMan->getSelectedTrack(0)->addAudioRegion(QUuid::createUuid().toString().toStdString());
            tl->addRegion(newAudioRegion);
            newAudioRegion->loadFile(path.toStdString(), true);
        }
    } else {

    }



}

Track* ArrangeWidget::addAudioTrack(Track *track, std::string uuid) {
    if (!track) {
        track = audioMan->addTrack(uuid);
    }
    tl->addTrack(track);
    mixer->addChannel(track);
    audioMan->setTrackSelected(track, true);
    return track;
}


void ArrangeWidget::on_soloDisableButton_clicked()
{
    //mixer->clearAll();
    audioMan->clearAll();
}

void ArrangeWidget::setMixer(Mixer *_mixer) {
    mixer = _mixer;
}

void ArrangeWidget::setHZoomFactor(int hZoomFactor) {
    ui->zoomSlider->setValue(hZoomFactor);
}
