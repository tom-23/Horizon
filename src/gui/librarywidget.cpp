#include "librarywidget.h"
#include "ui_librarywidget.h"
#include <QtGui>
#include <QFileSystemModel>
#include <QTreeWidgetItem>

LibraryWidget::LibraryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibraryWidget)
{
    ui->setupUi(this);
    QTreeWidgetItem *audioEffects = new QTreeWidgetItem();
    audioEffects->setText(0, "Audio Effects");
    QTreeWidgetItem *reverbEffect = new QTreeWidgetItem();
    reverbEffect->setText(0, "Reverb");
    QTreeWidgetItem *eqEffect = new QTreeWidgetItem();
    eqEffect->setText(0, "Channel EQ");
    QTreeWidgetItem *delayEffect = new QTreeWidgetItem();
    delayEffect->setText(0, "Tape Delay");
    audioEffects->addChild(reverbEffect);
    audioEffects->addChild(eqEffect);
    audioEffects->addChild(delayEffect);
    ui->libraryTree->addTopLevelItem(audioEffects);

}

LibraryWidget::~LibraryWidget()
{
    delete ui;
}
