#include "audioeffectwindow.h"
#include "ui_audioeffectwindow.h"

// TODO: this feature needs to be implemted but life's too short and this project need to be handed in ASAP.

AudioEffectWindow::AudioEffectWindow(QWidget *parent, AudioEffect *_audioEffect) :
    QDialog(parent),
    ui(new Ui::AudioEffectWindow)
{
    ui->setupUi(this);
    audioEffect = _audioEffect;
    //ui->audioEffectName->setText(QString::fromStdString(audioEffect->getFriendlyName()));
    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
}

AudioEffectWindow::~AudioEffectWindow()
{
    delete ui;
}

void AudioEffectWindow::setEffectUI(QWidget *uiWidget) {
    if (ui->effect_container->layout()->isEmpty() == false) {
        ui->effect_container->layout()->removeItem(ui->effect_container->layout()->itemAt(0));
    }
    ui->effect_container->layout()->addWidget(uiWidget);
    this->adjustSize();
    this->window()->resize(this->window()->minimumSize());
}

void AudioEffectWindow::on_enabledButton_toggled(bool checked)
{
    if (checked) {
        ui->enabledButton->setText("On");
    } else {
        ui->enabledButton->setText("Off");
    }
}
