#include "trackcontrolswidget.h"
#include "ui_trackcontrolswidget.h"
#include <QPalette>
#include <QColor>
#include <QRandomGenerator>
#include <QMenu>
#include <QContextMenuEvent>

TrackControlsWidget::TrackControlsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrackControlsWidget)
{
    ui->setupUi(this);

    QColor color = QColor::fromRgb(QRandomGenerator::global()->generate());
    QString style = QString("#trackColor { background-color: rgb(%1,%2,%3); }").arg(color.red()).arg(color.green()).arg(color.blue());
    ui->trackColor->setStyleSheet(style);

}

TrackControlsWidget::~TrackControlsWidget()
{
    delete ui;
}


void TrackControlsWidget::on_TrackControlsWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.setTitle("Hello");

   menu.setAutoFillBackground(true);

   menu.addAction("New Track...");
   menu.addAction("New Group...");
   menu.addAction("New Bus...");
   menu.addSeparator();
   menu.addAction("Cut")->setShortcut(QKeySequence::Cut);
   menu.addAction("Copy")->setShortcut(QKeySequence::Copy);
   menu.addAction("Paste")->setShortcut(QKeySequence::Copy);
   menu.addSeparator();
   menu.addAction("Rename...");
   menu.addAction("Choose Colour");
   menu.setWindowFlags(menu.windowFlags() | Qt::CustomizeWindowHint);
   menu.exec(mapToGlobal(pos));

}
