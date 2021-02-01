#include "mixerchannelwidget.h"
#include "ui_mixerchannelwidget.h"


MixerChannelWidget::MixerChannelWidget(QWidget *parent, Track *_track) :
    QWidget(parent),
    ui(new Ui::MixerChannelWidget)
{
    ui->setupUi(this);
    track = _track;
    ui->number->setText(QString::number(track->getIndex() + 1));
    ui->muteButton->setChecked(track->getMute());
    ui->gainSlider->setValue(int(track->getGain() * 100));
    ui->panSlider->setValue(int(track->getPan() * 100));

    updateColor();

    QGraphicsScene *scene = new QGraphicsScene(ui->channelMeter);
    mtr = new MeterWidget(ui->channelMeter, 0, 110, dialogs::getThemeManager()->getColor("borders"));
    scene->addItem(mtr);
    ui->channelMeter->setScene(scene);

    //uiTimer = new QTimer();
    //uiTimer->connect(uiTimer, &QTimer::timeout, this, QOverload<>::of(&MixerChannelWidget::uiUpdate));
    //uiTimer->start(60);

    updateTimeout = 0;

    ui->channelColor->setVisible(true);
    ui->gainSlider->setVisible(true);
    ui->channelMeter->setVisible(true);

    this->setStyleSheet("");

    mtr->setPwrValue(0, 0);
    mtr->setRMSValue(0, 0);

    mtr->setSize(ui->channelMeter->width(), ui->channelMeter->height());

}

void MixerChannelWidget::setHZoomFactor(int value) {
    hZoomFactor = value;
    this->setMaximumWidth(value);
    this->setMinimumWidth(value - 1);
}

MixerChannelWidget::~MixerChannelWidget()
{
    delete ui;
}

void MixerChannelWidget::on_gainSlider_valueChanged(int value)
{

    if (float(value) / 100 == -69.0) {
        track->setGain(-1000.0);
        ui->gainLabel->setText("-inf dB");
    } else {
        track->setGain(float(value) / 100);
        ui->gainLabel->setText(QString::number(float(value) / 100) + " dB");
    }

}

void MixerChannelWidget::setSelected(bool selected) {
    if (selected == true) {
        ui->channelContainer->setStyleSheet("#channelContainer { background-color: rgba(255,255,255, 0.05); }");
    } else {
        ui->channelContainer->setStyleSheet("");
        ui->effectsList->clearSelection();
    }
}

void MixerChannelWidget::mousePressEvent(QMouseEvent *event) {
    if (shiftDown == true) {
       // if (track->getAudioManager()->getSelectedTrack(0) == nullptr) {
       //     track->getAudioManager()->setTrackSelected(track, true);
       // } else {
        //    track->getAudioManager()->setTrackRangeSelected(track->getAudioManager()->getSelectedTrack(0), track);
       // }
    } else {
        track->getAudioManager()->setTrackSelected(track, true);
        ui->effectsList->clearSelection();
    }
}

void MixerChannelWidget::keyPressEvent(QKeyEvent *event) {
    qDebug() << "Key press";
    switch (event->key()) {
    case(Qt::Key::Key_Shift): {
            qDebug() << "SHIFT DOWN";
            shiftDown = true;
            break;
        }
    }
    QWidget::keyPressEvent(event);
}

void MixerChannelWidget::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
    case(Qt::Key::Key_Shift): {
            qDebug() << "SHIFT UP";
            shiftDown = false;
            break;
        }
    }
    QWidget::keyReleaseEvent(event);
}

void MixerChannelWidget::on_muteButton_toggled(bool checked)
{
    track->setMute(checked);
    track->getAudioManager()->session->setTrackMute(QString::fromStdString(track->getUUID()), checked);
}

void MixerChannelWidget::uiUpdate() {

    mtr->setSize(ui->channelMeter->width(), ui->channelMeter->height());

    if (track->getAudioRegionListCount() != 0) {
        std::vector<int> Lvalue = track->getLMeterData();
        std::vector<int> Rvalue = track->getRMeterData();

        int LRMSValue = Lvalue[0] + 100;
        int RRMSValue = Rvalue[0] + 100;

        if (lastLRMS == LRMSValue) {
            if (uiLRMS > 0) {
                uiLRMS = uiLRMS -2;
            }
        } else {
            if (uiLRMS < LRMSValue) {
                uiLRMS = LRMSValue;
            } else {
                if (uiLRMS > 0) {
                    uiLRMS = uiLRMS -2;
                }
            }

        }

        lastLRMS = LRMSValue;

        if (lastRRMS == RRMSValue) {
            if (uiRRMS > 0) {
                uiRRMS = uiRRMS -2;
            }
        } else {
            if (uiRRMS < RRMSValue) {
                uiRRMS = RRMSValue;
            } else {
                if (uiRRMS > 0) {
                    uiRRMS = uiRRMS -2;
                }
            }

        }

        lastRRMS = RRMSValue;

        mtr->setRMSValue(uiLRMS, uiRRMS);
        //mtr->setPwrValue(Lvalue[1] + 100, Rvalue[1] + 100);


        mtr->update();
    } else {

    }

    if (track->getMute() == true && ui->muteButton->isChecked() == false) {
        ui->muteButton->setChecked(true);
    } else if (track->getMute() == false && ui->muteButton->isChecked() == true) {
        ui->muteButton->setChecked(false);
    }



    if (updateTimeout == 19) {
        updateTimeout = 0;

        if (track->getPan() != float(ui->panSlider->value()) / 100) {
            ui->panSlider->setValue(int(track->getPan() * 100));
            lastPan = track->getPan();
        } else {
            if (track->getPan() != lastPan) {
                track->getAudioManager()->session->setTrackPan(QString::fromStdString(track->getUUID()), track->getPan());
                lastPan = track->getPan();
            }
        }

        if (track->getGain() != float(ui->gainSlider->value()) / 100) {
            ui->gainSlider->setValue(int(track->getGain() * 100));
            lastGain = track->getGain();

        } else {
            if (track->getGain() != lastGain) {
                track->getAudioManager()->session->setTrackGain(QString::fromStdString(track->getUUID()), track->getGain());
                lastGain = track->getGain();
            }
        }

    } else {
        updateTimeout = updateTimeout + 1;
    }

}

void MixerChannelWidget::on_panSlider_valueChanged(int value)
{
    track->setPan(float(value) / 100);
}

void MixerChannelWidget::updateColor() {
    QColor color = track->getColor();
    QString style = QString("#channelColor { background-color: rgb(%1,%2,%3); }").arg(color.red()).arg(color.green()).arg(color.blue());
    ui->channelColor->setStyleSheet(style);
}

void MixerChannelWidget::on_effectsList_itemSelectionChanged()
{
    track->addAudioEffect(effectType::compressor);
}

void MixerChannelWidget::on_effectsList_clicked(const QModelIndex &index)
{
    if (track->getSelected() == false) {
        track->getAudioManager()->setTrackSelected(track, true);
    }
}

void MixerChannelWidget::on_effectsList_customContextMenuRequested(const QPoint &pos)
{

}

void MixerChannelWidget::resizeEvent(QResizeEvent* event) {

}


