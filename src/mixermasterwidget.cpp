#include "mixermasterwidget.h"
#include "ui_mixermasterwidget.h"

MixerMasterWidget::MixerMasterWidget(QWidget *parent, Track *_track) :
    QWidget(parent),
    ui(new Ui::MixerMasterWidget)
{
    ui->setupUi(this);
    // Setup ui information
    ui->muteButton->setChecked(track->getMute());
    ui->gainSlider->setValue(int(track->getGain() * 100));
    ui->panSlider->setValue(int(track->getPan() * 100));


    // Setup meter
    ui->channelMeter->setMinMax(0, 110);
    ui->channelMeter->setPwrValue(0, 0);
    ui->channelMeter->setRMSValue(0, 0);
}

MixerMasterWidget::~MixerMasterWidget()
{
    delete ui;
}

void MixerMasterWidget::on_gainSlider_valueChanged(int value)
{
    if (float(value) / 100 == -69.0) {
        track->setGain(-1000.0);
        ui->gainLabel->setText("-inf dB");
    } else {
        track->setGain(float(value) / 100);
        ui->gainLabel->setText(QString::number(float(value) / 100) + " dB");
    }
}

void MixerMasterWidget::on_muteButton_toggled(bool checked)
{
    track->setMute(checked);
    track->getAudioManager()->session->setTrackMute(QString::fromStdString(track->getUUID()), checked);
}

void MixerMasterWidget::uiUpdate() {


    if (track->getAudioRegionListCount() != 0) { // Don't bother updating the meters if there are no regions.

        std::vector<int> Lvalue = track->getLMeterData();
        std::vector<int> Rvalue = track->getRMeterData();

        int LRMSValue = Lvalue[0] + 100;
        int RRMSValue = Rvalue[0] + 100;

        int LPWRValue = Lvalue[1] + 100;
        int RPWRValue = Rvalue[1] + 100;

        if (track->isLSilent()) {
            if (uiLRMS > 0) {
                uiLRMS = uiLRMS -2;
            }
            if (uiLPWR > 0) {
                uiLPWR = uiLPWR -2;
            }
        } else {
            if (uiLRMS < LRMSValue) {
                uiLRMS = LRMSValue;
            } else {
                if (uiLRMS > 0) {
                    uiLRMS = uiLRMS -2;
                }
            }
            if (uiLPWR < LPWRValue) {
                uiLPWR = LPWRValue;
            } else {
                if (uiLPWR > 0) {
                    uiLPWR = uiLPWR -2;
                }
            }

        }

        lastLRMS = LRMSValue;
        //lastLPWR = LPWRValue;

        if (track->isRSilent()) {
            if (uiRRMS > 0) {
                uiRRMS = uiRRMS -2;
            }
            if (uiRPWR > 0) {
                uiRPWR = uiRPWR -2;
            }
        } else {
            if (uiRRMS < RRMSValue) {
                uiRRMS = RRMSValue;
            } else {
                if (uiRRMS > 0) {
                    uiRRMS = uiRRMS -2;
                }
            }
            if (uiRPWR < RPWRValue) {
                uiRPWR = RPWRValue;
            } else {
                if (uiRPWR > 0) {
                    uiRPWR = uiRPWR -2;
                }
            }
        }

        lastRRMS = RRMSValue;

        ui->channelMeter->setRMSValue(uiLRMS, uiRRMS);
        ui->channelMeter->setPwrValue(uiLPWR, uiRPWR);
        ui->channelMeter->update();
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

void MixerMasterWidget::on_panSlider_valueChanged(int value)
{
    track->setPan(float(value) / 100);
}
