#include "arrangewidget.h"
#include "ui_arrangewidget.h"
#include <QScrollBar>
#include <QRegion>
#include <QDebug>
#include <QPainter>

Timeline *tl;

ArrangeWidget::ArrangeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArrangeWidget)
{
    ui->setupUi(this);

    connect(ui->trackRegions->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->trackControls->verticalScrollBar(), SLOT(setValue(int)));
    connect(ui->trackControls->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->trackRegions->verticalScrollBar(), SLOT(setValue(int)));


    connect(ui->overview, SIGNAL(valueChanged(int)), ui->trackRegions->horizontalScrollBar(), SLOT(setValue(int)));
    connect(ui->trackRegions->horizontalScrollBar(), SIGNAL(valueChanged(int)), ui->overview, SLOT(setValue(int)));
    connect(ui->trackRegions->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), ui->overview, SLOT(setRange(int, int)));

    connect(ui->trackRegions->horizontalScrollBar(), SIGNAL(valueChanged(int)), ui->trackRuler->horizontalScrollBar(), SLOT(setValue(int)));

    tl = new Timeline(ui->trackRegions, ui->trackRuler, ui->trackControlsContents);



}

ArrangeWidget::~ArrangeWidget()
{
    tl->~Timeline();
    delete ui;
}

void ArrangeWidget::on_pushButton_4_clicked()
{
   const QRect capturerect = QRect(QPoint(0, 0), ui->trackRegions->size());
    qDebug() << ui->trackRegions->size().width();
//
    QPixmap capture = ui->trackRegions->grab(capturerect);
    capture = capture.scaled(ui->overview->size(), Qt::IgnoreAspectRatio);

    capture.save(QApplication::applicationDirPath() + "/image.png", "PNG");

    ui->overview->setStyleSheet("background-image: url('image.png');");
    ui->overview->repaint();
    ui->overview->update();
}

void ArrangeWidget::on_pushButton_5_clicked()
{

    tl->addTrack(tl->trackCount);


}

void ArrangeWidget::resizeEvent(QResizeEvent *event)
{
    const QRect capturerect = QRect(QPoint(0, 0), ui->trackRegions->size());
     qDebug() << ui->trackRegions->size().width();

     QPixmap capture = ui->trackRegions->grab(capturerect);
     capture = capture.scaled(ui->overview->size(), Qt::IgnoreAspectRatio);

     capture.save(QApplication::applicationDirPath() + "/image.png", "PNG");

     ui->overview->setStyleSheet("background-image: url('image.png');");
     ui->overview->repaint();
     ui->overview->update();
}


void ArrangeWidget::on_zoomSlider_valueChanged(int value)
{
    tl->setHZoomFactor(value);
    qDebug() << value;
}

void ArrangeWidget::on_spinBox_valueChanged(int arg1)
{
    tl->setBarAmount(arg1);
}

void ArrangeWidget::on_spinBox_2_valueChanged(int arg1)
{
    tl->setBarLength(arg1);
}
