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

    ui->libraryTree->setAnimated(true);

    factoryContent = {{"/core/FactoryContent/Samples", "Samples", "audio"}, {"/core/FactoryContent/Drums", "Drums", "drums"}};

    folderIcon = QIcon();
    folderIcon.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/folder.svg"), QSize(16,16), QIcon::Normal);
    folderIcon.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/folder_selected.svg"), QSize(16,16), QIcon::Selected);
    folderIcon.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/folder_open.svg"), QSize(16,16), QIcon::Normal, QIcon::On);
    folderIcon.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/folder_open_selected.svg"), QSize(16,16), QIcon::Selected, QIcon::On);


    samplesIcon = QIcon();
    samplesIcon.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/audio.svg"), QSize(16,16), QIcon::Normal);
    samplesIcon.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/audio_selected.svg"), QSize(16,16), QIcon::Selected);

    refesh();

}

LibraryWidget::~LibraryWidget()
{
    delete ui;
}

void LibraryWidget::refesh() {



    ui->libraryTree->clear();

    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0, prefs->libraryDirList.size() + factoryContent.size());
    ui->indexingContainer->setVisible(true);

    refreshFactoryContent();

    itemsToIndex = 0;
    for (int i = 0; i < prefs->libraryDirList.size(); i++) {
        QDir folderDir(prefs->libraryDirList.at(i));

        IndexingThread *indexingThread = new IndexingThread(this, folderDir);
        indexingThread->folderIcon = folderIcon;
        indexingThread->samplesIcon = samplesIcon;

        connect(indexingThread, &IndexingThread::resultReady, this, &LibraryWidget::indexComplete);
        indexingThread->start();
    }
}

void LibraryWidget::refreshFactoryContent() {

    for (int i = 0; i < factoryContent.size(); i++) {
        QDir samplesFolderDir(bundleLoc + factoryContent.at(i).at(0));

        IndexingThread *indexingThread = new IndexingThread(this, samplesFolderDir, true);
        QIcon topLevelIcon;
        topLevelIcon.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/" + factoryContent.at(i).at(2) + ".svg"), QSize(16,16), QIcon::Normal);
        topLevelIcon.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/" + factoryContent.at(i).at(2) + "_selected.svg"), QSize(16,16), QIcon::Selected);
        indexingThread->topLevelIcon = topLevelIcon;
        indexingThread->folderIcon = folderIcon;
        indexingThread->samplesIcon = samplesIcon;
        indexingThread->topLevelText = factoryContent.at(i).at(1);

        connect(indexingThread, &IndexingThread::resultReady, this, &LibraryWidget::indexComplete);
        indexingThread->start();
    }
}

void LibraryWidget::on_refeshButton_clicked()
{
    refesh();
}

void LibraryWidget::indexComplete(QTreeWidgetItem *parentItem) {
    ui->libraryTree->addTopLevelItem(parentItem);
    if (itemsToIndex == ui->progressBar->maximum() - 1) {
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


void LibraryWidget::setCollapsed(bool collapsed) {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "maximumWidth");
    animation->setDuration(500);

    if (!collapsed) {
        setMinimumWidth(0);
        animation->setStartValue(400);
        animation->setEndValue(0);
    } else {
        setMinimumWidth(0);
        setVisible(true);
        animation->setStartValue(0);
        animation->setEndValue(400);
    }

    connect(animation, &QPropertyAnimation::finished, [collapsed, this] () {
        if (!collapsed) {
            this->setVisible(false);
        }
    });
    animation->setEasingCurve(QEasingCurve::InOutSine);
    animation->start();
}
