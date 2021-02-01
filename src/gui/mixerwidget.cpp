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


void MixerWidget::setCollapsed(bool collapsed) {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "maximumHeight");
    animation->setDuration(500);

    if (!collapsed) {
        setMinimumHeight(0);
        animation->setStartValue(500);
        animation->setEndValue(0);
    } else {
        setMinimumHeight(0);
        setVisible(true);
        animation->setStartValue(0);
        animation->setEndValue(500);
    }

    connect(animation, &QPropertyAnimation::finished, [collapsed, this] () {
        if (!collapsed) {
            this->setVisible(false);
        }
    });
    animation->setEasingCurve(QEasingCurve::InOutSine);
    animation->start();
}
