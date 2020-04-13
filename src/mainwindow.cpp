#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LabSound/LabSound.h"
#include "grid.h"



using namespace lab;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ArrangeWidget* ar;
    ar = new ArrangeWidget(this);
    ui->content->layout()->addWidget(ar);
    ar->show();

    InfoWidget* iw;
    iw = new InfoWidget(this);
    ui->content->layout()->addWidget(iw);
    iw->show();

    LibraryWidget* lb;
    lb = new LibraryWidget(this);
    ui->content->layout()->addWidget(lb);
    lb->show();

    EffectWidget* ew;
    ew = new EffectWidget(iw);
    iw->addEffect(ew);

    AudioGrid *ag = new AudioGrid();

}

MainWindow::~MainWindow()
{
    delete ui;

}
template <typename Duration>
void Wait(Duration duration)
    {
        std::mutex wait;
        std::unique_lock<std::mutex> lock(wait);
        std::condition_variable cv;
        cv.wait_for(lock, duration);
    }


std::shared_ptr<AudioBus> musicClip;

std::shared_ptr<OscillatorNode> oscillator;
std::shared_ptr<SampledAudioNode> musicClipNode;
//std::shared_ptr<DynamicsCompressorNode> compressorNode;
std::shared_ptr<ConvolverNode> verb;
std::shared_ptr<GainNode> gain;



void MainWindow::on_playButton_clicked()
{
    audioMan = new AudioManager(this);
}
