#include "compressoreffectwindow.h"
#include "ui_compressoreffectwindow.h"

CompressorEffectWindow::CompressorEffectWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompressorEffectWindow)
{
    ui->setupUi(this);
    HorizonDial *dial = new HorizonDial("dB", this, 0, 99);
    ui->inputLayout->insertWidget(1, dial);

    //QPixmap pixmap;
    //pixmap.load(":/reverb/effects/reverb/background.svg");
    //ui->dial->setTexture(pixmap);
}

CompressorEffectWindow::~CompressorEffectWindow()
{
    delete ui;
}
