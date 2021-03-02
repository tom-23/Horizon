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
    // creating a new timeline object and passing in requred ui elements. this could of been implemented better.
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
    // when the zoom slider is moved, we zoom in the timeline.
    tl->setHZoomFactor(value, ui->zoomSlider);
}


void ArrangeWidget::setAudioManager(AudioManager &_audioMan) {
    audioMan = &_audioMan;
}

void ArrangeWidget::importAudio(QString path) {
    // checking to see if a track is selected
    if (audioMan->getSelectedTrack(0) != nullptr) {
        // ensure we have a path (validation)
        if (path == "") {
            // we can't load audio if we're playing back audio
            if (audioMan->isPlaying) {

                // if we are playing back audio, show a message box asking the user if they wan't to cancel playback.
                if (dialogs::MessageDialog::show("Importing audio stops playback", "This action will pause audio. Are you sure you want to continue?",
                                                 dialogs::MessageDialog::info, dialogs::MessageDialog::yesNo) == 3) {
                    // if they pressed no, we return out of the function
                    return;
                } else {
                    // if they pressed yes, pause.
                    audioMan->pause();
                }
            }
            // create a new file open dialog setting the filter to supported audio files.
            path = QFileDialog::getOpenFileName(this,
                                                tr("Load Audio"), QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
                                                tr("Supported Audio Files (*.wav *.mp3 *.flac)"));
        }

        // ensuring the user didn't click on the cancel button on the file dialog
        if (path.toStdString() != "") {

            // we then create a new audio region object by calling a function on the selected track object.
            // we neeed to create a UUID to identify the region when serializing and using real time collaboration.
            AudioRegion *newAudioRegion = audioMan->getSelectedTrack(0)->addAudioRegion(QUuid::createUuid().toString().toStdString());
            // setting the locaiton of the region to the playhead's current location
            newAudioRegion->setGridLocation(audioMan->getCurrentGridTime());
            // add the region to display on the timeline
            tl->addRegion(newAudioRegion);
            // load the audio file on the new audio region
            newAudioRegion->loadFile(path.toStdString(), true);
        }
    } else {
        // no selected track? don't do anything
    }
}

Track* ArrangeWidget::addAudioTrack(Track *track, std::string uuid) {
    // TODO: this could be implemented in a simular way we do when creating a new audio region
    // passed in a track object and uuid
    if (!track) {
        // if we never had a track passed in, create a new track using the uuid to identify.
        track = audioMan->addTrack(uuid);
    }
    // add the track to the timeline to display on the ui
    tl->addTrack(track);
    // add the track to the mixer ui
    mixer->addChannel(track);
    // select the track
    audioMan->setTrackSelected(track, true);
    return track;
}

void ArrangeWidget::on_soloDisableButton_clicked()
{
    // TODO: implement me!
}

void ArrangeWidget::setMixer(Mixer *_mixer) {
    // sets the current mixer object
    mixer = _mixer;
}

void ArrangeWidget::setHZoomFactor(int hZoomFactor) {
    // this would only serve a purpose if I implemented trackpad zoom.
    ui->zoomSlider->setValue(hZoomFactor);
}

void ArrangeWidget::on_centerPlayheadButton_toggled(bool checked)
{
    // if this button is checked, we scroll to center the playhead.
    tl->centerPlayhead = checked;
}

void ArrangeWidget::on_hScroll_actionTriggered(int action)
{
    // if the user tried to scroll when centerplayhead is enabled, we disable it to allow the user to control the scroll bar position.
    if (action == 7) {
        ui->centerPlayheadButton->setChecked(false);
    }
}

void ArrangeWidget::on_pushButton_3_clicked()
{
    // unused
}

void ArrangeWidget::on_pushButton_3_toggled(bool checked)
{
    //unused
}
