#include "trackcontrolswidget.h"
#include "ui_trackcontrolswidget.h"
#include <QPalette>
#include <QColor>
#include <QRandomGenerator>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDebug>

TrackControlsWidget::TrackControlsWidget(QWidget *parent, Track *_track) :
    QWidget(parent),
    ui(new Ui::TrackControlsWidget)
{
    ui->setupUi(this);
    track = _track;
    shiftDown = false;
    QColor color = QColor::fromRgb(QRandomGenerator::global()->generate());
    QString style = QString("#trackColor { background-color: rgb(%1,%2,%3); }").arg(color.red()).arg(color.green()).arg(color.blue());
    ui->trackColor->setStyleSheet(style);
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);

}

TrackControlsWidget::~TrackControlsWidget()
{
    delete ui;
}


void TrackControlsWidget::on_TrackControlsWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
   menu.setAutoFillBackground(true);

   menu.addAction("New Track...");
   menu.addAction("New Group...");
   menu.addAction("New Bus...");
   menu.addSeparator();
   menu.addAction("Cut")->setShortcut(QKeySequence::Cut);
   menu.addAction("Copy")->setShortcut(QKeySequence::Copy);
   menu.addAction("Paste")->setShortcut(QKeySequence::Copy);
   menu.addSeparator();
   menu.addAction("Rename...");
   menu.addAction("Choose Colour");
   menu.setWindowFlags(menu.windowFlags() | Qt::CustomizeWindowHint);
   menu.exec(mapToGlobal(pos));

}

void TrackControlsWidget::mousePressEvent(QMouseEvent *event) {

    if (shiftDown == true) {
       // if (track->getAudioManager()->getSelectedTrack(0) == nullptr) {
       //     track->getAudioManager()->setTrackSelected(track, true);
       // } else {
        //    track->getAudioManager()->setTrackRangeSelected(track->getAudioManager()->getSelectedTrack(0), track);
       // }
    } else {
        track->getAudioManager()->setTrackSelected(track, true);
    }
    qDebug() << "Selected track";


}

void TrackControlsWidget::mouseReleaseEvent(QMouseEvent *event) {
    qDebug() << "Release";

}
void TrackControlsWidget::setSelected(bool selected) {
    if (selected == true) {
        ui->trackContainer->setStyleSheet("#trackContainer { background-color: rgba(255,255,255, 0.05); }");
    } else {
        ui->trackContainer->setStyleSheet("");
    }
}

void TrackControlsWidget::keyPressEvent(QKeyEvent *event) {
    qDebug() << "Key press";
    switch (event->key()) {
    case(Qt::Key::Key_Shift): {
            qDebug() << "SHIFT DOWN";
            shiftDown = true;
        }
    }
}

void TrackControlsWidget::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
    case(Qt::Key::Key_Shift): {
            qDebug() << "SHIFT UP";
            shiftDown = false;
        }
    }
}


void TrackControlsWidget::on_muteButton_toggled(bool checked)
{
    track->setMute(checked);
}
