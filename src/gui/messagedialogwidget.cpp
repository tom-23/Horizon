#include "messagedialogwidget.h"
#include "ui_messagedialogwidget.h"

MessageDialogWidget::MessageDialogWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialogWidget)
{
    ui->setupUi(this);
}

MessageDialogWidget::~MessageDialogWidget()
{
    delete ui;
}
