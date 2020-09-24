#include "trackinfowidget.h"
#include "ui_trackinfowidget.h"

TrackInfoWidget::TrackInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrackInfoWidget)
{
    ui->setupUi(this);
}

TrackInfoWidget::~TrackInfoWidget()
{
    delete ui;
}
