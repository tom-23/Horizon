#include "audioeffectwindow.h"
#include "ui_audioeffectwindow.h"

AudioEffectWindow::AudioEffectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AudioEffectWindow)
{
    ui->setupUi(this);
}

AudioEffectWindow::~AudioEffectWindow()
{
    delete ui;
}
