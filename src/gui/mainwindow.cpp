#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    themeMan = new ThemeManager(this, QString::fromUtf8(util::getResourceBundle().c_str()) + "/themes/default-dark.json");

    updateIconThemes();


    debug::out(3, "Loading audio manager...");


    arrangeWidget = new ArrangeWidget(this);
    audioMan = std::make_unique<AudioManager>(*arrangeWidget->tl);

    arrangeWidget->setAudioManager(*audioMan);

    debug::out(3, "Setting BPM");
    audioMan->setBPM(150.0);
    audioMan->setDivision(4);
    audioMan->setLookAhead(0.05);

    debug::out(3, "Loaded audio manager with default settings!");

    
    ui->content->layout()->addWidget(arrangeWidget);
    arrangeWidget->show();
    arrangeWidget->tl->setColorTheme(themeMan);
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
    arrangeWidget->tl->setPlayheadLocation(audioMan->getCurrentGridTime());
    //qDebug() << audioMan->getCurrentGridTime();
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_playButton_clicked()
{
    if (audioMan->isPlaying == false) {
        uiTimer->start(30);
        audioMan->play();
    } else {
        uiTimer->stop();
        audioMan->pause();
    }

}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *ab = new AboutDialog(this, themeMan);
    ab->show();
}


void MainWindow::on_stopButton_clicked()
{
    audioMan->stop();
    uiTimer->stop();
    arrangeWidget->tl->setPlayheadLocation(0.0);
}

void MainWindow::updateIconThemes() {
    ui->playButton->setStyleSheet("image: url('" + themeMan->colorizeSVG(":/svg/svg/play.svg") + "');");
    ui->stopButton->setStyleSheet("image: url('" + themeMan->colorizeSVG(":/svg/svg/stop.svg") + "');");
    ui->recordButton->setStyleSheet("image: url('" + themeMan->colorizeSVG(":/svg/svg/record.svg") + "');");
}


