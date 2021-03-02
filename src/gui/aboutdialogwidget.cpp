#include "aboutdialogwidget.h"
#include "ui_aboutdialogwidget.h"
#include <QDesktopServices>
#include <QGraphicsItem>
#include <QUrl>

AboutDialogWidget::AboutDialogWidget(QWidget *parent, QString svgPath) :
    QDialog(parent),
    ui(new Ui::AboutDialogWidget)
{
    // set the logo
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
    // this button would open a link to the website
    QDesktopServices::openUrl(QUrl(""));
}

void AboutDialogWidget::on_pushButton_2_clicked()
{
    this->close();
}
