    #include "mixerwidget.h"
#include "ui_mixerwidget.h"

#include "mixerchannelwidget.h"

MixerWidget::MixerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MixerWidget)
{
    ui->setupUi(this);

    mixer = new Mixer(ui->mixerContainer);
    mixer->setHZoomFactor(ui->zoomSlider->value());

   // MixerChannelWidget *mcw = new MixerChannelWidget(this);

    //QHBoxLayout* channelLayout = qobject_cast<QHBoxLayout*>(ui->mixerContainerWidget->layout());
    //channelLayout->insertWidget(channelLayout->count() - 1, mcw);


}

MixerWidget::~MixerWidget()
{
    delete ui;
}

void MixerWidget::on_zoomSlider_valueChanged(int value)
{
    mixer->setHZoomFactor(value);
}
