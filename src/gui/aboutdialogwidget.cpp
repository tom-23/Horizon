#include "aboutdialogwidget.h"
#include "ui_aboutdialogwidget.h"
#include <QDesktopServices>
#include <QGraphicsItem>
#include <QUrl>

AboutDialogWidget::AboutDialogWidget(QWidget *parent, QString svgPath) :
    QDialog(parent),
    ui(new Ui::AboutDialogWidget)
{
    ui->setupUi(this);
    ui->imageContainer->setStyleSheet("image: url('" + svgPath + "');");

    this->setModal(true);
    this->setWindowFlag(Qt::WindowCloseButtonHint);
}

AboutDialogWidget::~AboutDialogWidget()
{
    delete ui;
}

void AboutDialogWidget::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl("http://tom-butcher.ml:2233/git/tom/Horizon"));
}

void AboutDialogWidget::on_pushButton_2_clicked()
{
    this->close();
}
