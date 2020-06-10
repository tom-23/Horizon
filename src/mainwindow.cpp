#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LabSound/LabSound.h"
#include "grid.h"
#include "aboutdialog.h"



using namespace lab;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    themeMan = new ThemeManager(this, "../Resources/themes/Nautic.json");

    updateIconThemes();

    audioMan = new AudioManager(ar->tl);
    audioMan->setBPM(126.0);
    audioMan->setDivision(4);
    audioMan->setLookAhead(0.05);

    ar = new ArrangeWidget(this, audioMan);
    ui->content->layout()->addWidget(ar);
    ar->show();
    ar->tl->setColorTheme(themeMan);
    //InfoWidget* iw;
    //iw = new InfoWidget(this);
    //ui->content->layout()->addWidget(iw);
    //iw->show();

    LibraryWidget* lb;
    lb = new LibraryWidget(this);
    ui->content->layout()->addWidget(lb);
    lb->show();

    //EffectWidget* ew;
    //ew = new EffectWidget(iw);
    //iw->addEffect(ew);

    uiTimer = new QTimer(parent);
    connect(uiTimer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::uiUpdate));

}

void MainWindow::uiUpdate() {
    ar->tl->setPlayheadLocation(audioMan->getCurrentGridTime());
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_playButton_clicked()
{
    uiTimer->start(30);
    audioMan->play();
}

void MainWindow::on_pushButton_clicked()
{
    themeMan = new ThemeManager(this, "../Resources/themes/default-dark.json");
    themeMan->updateTheme();
    updateIconThemes();
    ar->tl->setColorTheme(themeMan);
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *ab = new AboutDialog(this, themeMan);
    ab->show();
}


void MainWindow::on_stopButton_clicked()
{
    uiTimer->stop();
    audioMan->stop();
}

void MainWindow::updateIconThemes() {
    ui->playButton->setStyleSheet("image: url('" + themeMan->colorizeSVG(":/svg/svg/play.svg") + "');");
    ui->stopButton->setStyleSheet("image: url('" + themeMan->colorizeSVG(":/svg/svg/stop.svg") + "');");
    ui->recordButton->setStyleSheet("image: url('" + themeMan->colorizeSVG(":/svg/svg/record.svg") + "');");
}
