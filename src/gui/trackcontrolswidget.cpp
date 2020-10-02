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
    //ui->trackMeter->setVisible(false);
    QColor color = QColor::fromRgb(QRandomGenerator::global()->generate());
    track->setColor(color);
    QString style = QString("#trackColor { background-color: rgb(%1,%2,%3); }").arg(color.red()).arg(color.green()).arg(color.blue());
    ui->trackColor->setStyleSheet(style);
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    ui->number->setText(QString::number(track->getAudioManager()->getTrackListCount()));

    uiTimer = new QTimer(parent);
    connect(uiTimer, &QTimer::timeout, this, QOverload<>::of(&TrackControlsWidget::uiUpdate));
    uiTimer->start(60);

    QGraphicsScene *scene = new QGraphicsScene(ui->trackMeter);
    mtr = new MeterWidget(ui->trackMeter, 0, 110, true);
    scene->addItem(mtr);
    ui->trackMeter->setScene(scene);


    ui->trackMeter->update();
    ui->trackMeter->repaint();


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
}


void TrackControlsWidget::changeColor() {
    ColorPickerWidget *colorPicker = new ColorPickerWidget(this);
    colorPicker->show();
}

void TrackControlsWidget::uiUpdate() {

    std::vector<int> Lvalue = track->getLMeterData();
    std::vector<int> Rvalue = track->getRMeterData();




    mtr->setRMSValue(Lvalue[0] + 100, Rvalue[0] + 100);

    mtr->setPwrValue(Lvalue[1] + 100, Rvalue[1] + 100);

    mtr->update();

    ui->peakdBLabel->setText(QString::number(track->peakdB) + " dB");


}

void TrackControlsWidget::on_armButton_clicked()
{

}

void TrackControlsWidget::on_peakdBLabel_clicked()
{
    track->peakdB = -100;
}
