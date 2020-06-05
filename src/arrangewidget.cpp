#include "arrangewidget.h"
#include "ui_arrangewidget.h"
#include <QScrollBar>
#include <QRegion>
#include <QDebug>
#include <QPainter>
#include <QImage>


ArrangeWidget::ArrangeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArrangeWidget)
{
    ui->setupUi(this);
    tl = new Timeline(this, ui->ruler, ui->trackControls, ui->timeline->layout());
    this->repaint();



    ui->overview->hide();




}


ArrangeWidget::~ArrangeWidget()
{
    tl->~Timeline();
    delete ui;
}

void ArrangeWidget::on_pushButton_4_clicked()
{


//
    //QImage capture = QImage(ui->trackRegions->scene()->sceneRect().toRect().size(), QImage::Format_RGB16);
    //QPainter painter(&capture);

    //ui->trackRegions->scene()->render(&painter);
    //painter.end();
    //capture = capture.scaled(ui->overview->size(), Qt::IgnoreAspectRatio);

    //capture.save(QApplication::applicationDirPath() + "/image.png", "PNG");



    //ui->overview->setStyleSheet("background-image: url('image.png');");
    //ui->overview->repaint();
    //ui->overview->update();
}

void ArrangeWidget::on_pushButton_5_clicked()
{

    tl->addTrack(tl->getTrackCount());


}

void ArrangeWidget::resizeEvent(QResizeEvent *event)
{

}


void ArrangeWidget::on_zoomSlider_valueChanged(int value)
{

    tl->setHZoomFactor(value, ui->zoomSlider);
}


void ArrangeWidget::on_pushButton_clicked()
{
    tl->addRegion(0);
}


void ArrangeWidget::on_zoomSlider_sliderMoved(int position)
{


}
