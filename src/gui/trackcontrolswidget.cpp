#include "trackcontrolswidget.h"
#include "ui_trackcontrolswidget.h"
#include <QPalette>

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
    updateColor();
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    ui->number->setText(QString::number(track->getIndex() + 1));
    ui->muteButton->setChecked(track->getMute());

    QGraphicsScene *scene = new QGraphicsScene(this);
    mtr = new MeterWidget(ui->trackMeterView, 0, 110, true);
    scene->addItem(mtr);
    ui->trackMeterView->setScene(scene);

   // uiTime

    uiTimer = new QTimer();
   // uiTimer->connect(uiTimer, &QTimer::timeout, this, QOverload<>::of(&TrackControlsWidget::uiUpdate));
   // uiTimer->start(60);

    ui->trackMeterView->update();
    ui->trackMeterView->repaint();

    mtr->setPwrValue(0, 0);
    mtr->setRMSValue(0, 0);
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

   QAction *chooseColor = new QAction("Choose Color", this);
   menu.addAction(chooseColor);
   connect(chooseColor, &QAction::triggered, this, &TrackControlsWidget::changeColor);

   QAction *remove= new QAction("Delete", this);
   menu.addAction(remove);
   connect(remove, &QAction::triggered, this, &TrackControlsWidget::removeSelf);
   //connect(chooseColor, &QAction::triggered, this, &TrackControlsWidget::changeColor);

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
    QWidget::keyPressEvent(event);
}

void TrackControlsWidget::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
    case(Qt::Key::Key_Shift): {
            qDebug() << "SHIFT UP";
            shiftDown = false;
        }
    }
    QWidget::keyReleaseEvent(event);
}


void TrackControlsWidget::on_muteButton_toggled(bool checked)
{
    track->setMute(checked);
    track->getAudioManager()->session->setTrackMute(QString::fromStdString(track->getUUID()), checked);
}


void TrackControlsWidget::changeColor() {
    ColorPickerWidget *colorPicker = new ColorPickerWidget(this);
    colorPicker->show();
}

void TrackControlsWidget::uiUpdate() {

    if (track->getAudioRegionListCount() != 0) {
    std::vector<int> Lvalue = track->getLMeterData();
    std::vector<int> Rvalue = track->getRMeterData();

    int LRMSValue = Lvalue[0] + 100;
    int RRMSValue = Rvalue[0] + 100;

    if (lastLRMS == LRMSValue) {
        if (uiLRMS > 0) {
            uiLRMS = uiLRMS -1;
        }
    } else {
        if (uiLRMS < LRMSValue) {
            uiLRMS = LRMSValue;
        } else {
            if (uiLRMS > 0) {
                uiLRMS = uiLRMS -1;
            }
        }

    }

    lastLRMS = LRMSValue;

    if (lastRRMS == RRMSValue) {
        if (uiRRMS > 0) {
            uiRRMS = uiRRMS -1;
        }
    } else {
        if (uiRRMS < RRMSValue) {
            uiRRMS = RRMSValue;
        } else {
            if (uiRRMS > 0) {
                uiRRMS = uiRRMS -1;
            }
        }

    }

    lastRRMS = RRMSValue;

    mtr->setRMSValue(uiLRMS, uiRRMS);
    //mtr->setPwrValue(Lvalue[1] + 100, Rvalue[1] + 100);

    mtr->update();
    ui->peakdBLabel->setText(QString::number(track->peakdB) + " dB");

    }

    if (track->getMute() == true && ui->muteButton->isChecked() == false) {
        ui->muteButton->setChecked(true);
    } else if (track->getMute() == false && ui->muteButton->isChecked() == true) {
        ui->muteButton->setChecked(false);
    }
}

void TrackControlsWidget::on_armButton_clicked()
{

}

void TrackControlsWidget::on_peakdBLabel_clicked()
{
    track->peakdB = -100;
}

void TrackControlsWidget::updateColor() {
    QColor color = track->getColor();
    QString style = QString("#trackColor { background-color: rgb(%1,%2,%3); }").arg(color.red()).arg(color.green()).arg(color.blue());
    ui->trackColor->setStyleSheet(style);
}

void TrackControlsWidget::removeSelf() {

    track->getAudioManager()->removeTrack(track);
}


void TrackControlsWidget::mouseDoubleClickEvent(QMouseEvent *event) {

}
