#include "indexingthread.h"

IndexingThread::IndexingThread(QObject *parent, QDir dir, bool _topLevelSpecial) : QThread(parent)
{
    parentDir = dir;
    topLevelSpecial = _topLevelSpecial;
}

void IndexingThread::run() {
    isTopLevelSet = false;
    QTreeWidgetItem *widgetItem = scanDir(parentDir);
    emit resultReady(widgetItem);
}

QTreeWidgetItem* IndexingThread::scanDir(QDir dir) {
    QTreeWidgetItem *folder = new QTreeWidgetItem();
    if (topLevelSpecial && topLevelSpecial) {
        isTopLevelSet = true;
        folder->setIcon(0, QIcon(samplesIcon));
    } else {
        folder->setIcon(0, QIcon(folderIcon));
    }

    folder->setText(0, dir.dirName());



    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList dirList = dir.entryList();
    foreach(QString dirName, dirList) {
        QString newPath = QString("%1/%2").arg(dir.absolutePath()).arg(dirName);
        folder->addChild(scanDir(QDir(newPath)));
    }

    dir.setNameFilters(QStringList() << "*.mp3" << "*.wav");
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    foreach(QString filename, dir.entryList()) {

        QFileInfo fileInfo(dir.path() + "/" + filename);
        QTreeWidgetItem *audioFile = new QTreeWidgetItem();
        audioFile->setText(0, fileInfo.fileName());
        audioFile->setText(1, fileInfo.filePath());

        QMimeDatabase db;
        QMimeType mime = db.mimeTypeForFile(fileInfo.filePath(), QMimeDatabase::MatchContent);

        if (mime.preferredSuffix() == "wav") {
            audioFile->setIcon(0, QIcon(wavIcon));
        } else if (mime.preferredSuffix() == "mp3") {
            audioFile->setIcon(0, QIcon(mp3Icon));
        }
        folder->addChild(audioFile);
    }

    return folder;
}
