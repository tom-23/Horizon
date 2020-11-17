#include "internalnotificationwidget.h"
#include "ui_internalnotificationwidget.h"

InternalNotificationWidget::InternalNotificationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InternalNotificationWidget)
{
    ui->setupUi(this);
}

InternalNotificationWidget::~InternalNotificationWidget()
{
    delete ui;
}
