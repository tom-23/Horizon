#include "trackcontrolswidget.h"
#include "ui_trackcontrolswidget.h"
#include <QPalette>
#include <QColor>
#include <QRandomGenerator>


TrackControlsWidget::TrackControlsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrackControlsWidget)
{
    ui->setupUi(this);

    QColor color = QColor::fromRgb(QRandomGenerator::global()->generate());
    QString style = QString("#color { background-color: rgb(%1,%2,%3); }").arg(color.red()).arg(color.green()).arg(color.blue());
    ui->color->setStyleSheet(style);

}

TrackControlsWidget::~TrackControlsWidget()
{
    delete ui;
}
