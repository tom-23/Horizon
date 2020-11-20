#include "librarywidget.h"
#include "ui_librarywidget.h"
#include <QtGui>
#include <QFileSystemModel>
#include <QTreeWidgetItem>

LibraryWidget::LibraryWidget(QWidget *parent, Preferences *_prefs, ArrangeWidget *_arrangeWidget) :
    QWidget(parent),
    ui(new Ui::LibraryWidget)
{
    ui->setupUi(this);
    prefs = _prefs;
    arrangeWidget = _arrangeWidget;

    ui->searchBox->setAttribute(Qt::WA_MacShowFocusRect, 0);
    #ifndef _WIN32
    bundleLoc = QString::fromUtf8(util::getResourceBundle().c_str());
    #else
    bundleLoc = QString::fromUtf8(util::getInstallDir().c_str());
    #endif
    refesh();

}

LibraryWidget::~LibraryWidget()
{
    delete ui;
}

void LibraryWidget::refesh() {


    folderIcon = dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/folder.svg");
    mp3Icon = dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/mp3.svg");
    wavIcon = dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/wav.svg");
    samplesIcon = dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/sample.svg");

    ui->libraryTree->clear();

    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0, prefs->libraryDirList.size());
    ui->indexingContainer->setVisible(true);

    refreshHorizonSamples();

    itemsToIndex = 0;
    for (int i = 0; i < prefs->libraryDirList.size(); i++) {
        QDir folderDir(prefs->libraryDirList.at(i));

        IndexingThread *indexingThread = new IndexingThread(this, folderDir);
        indexingThread->folderIcon = folderIcon;
        indexingThread->wavIcon = wavIcon;
        indexingThread->mp3Icon = mp3Icon;
        indexingThread->samplesIcon = samplesIcon;

        connect(indexingThread, &IndexingThread::resultReady, this, &LibraryWidget::indexComplete);
        indexingThread->start();
    }
}

void LibraryWidget::refreshHorizonSamples() {
    QDir folderDir(bundleLoc + "/core/Horizon Sample Pack");

    IndexingThread *indexingThread = new IndexingThread(this, folderDir, true);
    indexingThread->folderIcon = folderIcon;
    indexingThread->wavIcon = wavIcon;
    indexingThread->mp3Icon = mp3Icon;
    indexingThread->samplesIcon = samplesIcon;

    connect(indexingThread, &IndexingThread::resultReady, this, &LibraryWidget::indexComplete);
    indexingThread->start();
}

void LibraryWidget::on_refeshButton_clicked()
{
    refesh();
}

void LibraryWidget::indexComplete(QTreeWidgetItem *parentItem) {
    ui->libraryTree->addTopLevelItem(parentItem);
    if (itemsToIndex == prefs->libraryDirList.size()) {
        ui->indexingContainer->setVisible(false);
    } else {
        itemsToIndex = itemsToIndex + 1;
        ui->progressBar->setValue(itemsToIndex);
    }
}

void LibraryWidget::on_searchBox_textChanged(const QString &arg1)
{
    QList<QTreeWidgetItem*> itemlist = ui->libraryTree->findItems("", Qt::MatchContains|Qt::MatchRecursive, 0);
    foreach(QTreeWidgetItem* item, itemlist)
    {
        item->setHidden(true);
    }

    QList<QTreeWidgetItem*> searchlist = ui->libraryTree->findItems(arg1, Qt::MatchContains|Qt::MatchRecursive, 0);
    foreach(QTreeWidgetItem* item, searchlist)
    {
        showParent(item);
        item->setHidden(false);
    }
}

void LibraryWidget::showParent(QTreeWidgetItem *item) {
    if (!(ui->libraryTree->indexOfTopLevelItem(item) > -1)) {
        item->parent()->setHidden(false);
        showParent(item->parent());
    }

}

void LibraryWidget::on_libraryTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->text(1) != "") {
        arrangeWidget->importAudio(item->text(1));
    }

}
