#include "loadingdialogwidget.h"
#include "ui_loadingdialogwidget.h"
#include <QDebug>

LoadingDialogWidget::LoadingDialogWidget(QWidget *parent, int min, int max, QString message) :
    QDialog(parent),
    ui(new Ui::LoadingDialogWidget)
{
    ui->setupUi(this);
    ui->progressBar->setRange(min, max);
    ui->progressBar->setValue(0);
    ui->label_2->setText(message);
    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    this->setModal(true);

}

LoadingDialogWidget::~LoadingDialogWidget()
{
    delete ui;
}

void LoadingDialogWidget::updateProgress(int value) {
    ui->progressBar->setValue(value);
    ui->progressBar->update();
}

int LoadingDialogWidget::getProgress() {
    return ui->progressBar->value();
}

int LoadingDialogWidget::getMaxProgress() {
    return ui->progressBar->maximum();
}
