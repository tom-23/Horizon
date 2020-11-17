#include "renderwindow.h"
#include "ui_renderwindow.h"

RenderWindow::RenderWindow(QWidget *parent, AudioManager *_audioMan) :
    QDialog(parent),
    ui(new Ui::RenderWindow)
{
    ui->setupUi(this);
    audioMan = _audioMan;
    this->adjustSize();
    //ui->->setAttribute(Qt::WA_MacShowFocusRect, 0);
}

RenderWindow::~RenderWindow()
{
    delete ui;
}

void RenderWindow::on_cancel_clicked()
{
    this->close();
}

void RenderWindow::on_startButton_clicked()
{
    QString dialogFileName = QFileDialog::getSaveFileName(this,
            tr("Render Audio"), QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
            tr("PCM Wave Files (*.wav)"));
    if (dialogFileName != "") {
        this->close();
        audioMan->renderAudio(this->parent(), dialogFileName.toStdString(), ui->sampleRateBox->currentText().toInt(), 2);
    }
}
