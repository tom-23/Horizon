#include "splashscreen.h"
#include "ui_splashscreen.h"

SplashScreen::SplashScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);
}

SplashScreen::~SplashScreen()
{
    delete ui;
}

void SplashScreen::setText(QString text) {
    ui->messageLabel->setText(text);
}

void SplashScreen::setVersion(QString version) {
    ui->versionLabel->setText("version " + version);
}
