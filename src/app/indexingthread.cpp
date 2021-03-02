#include "indexingthread.h"

// here is the class which handles indexing of a thread.


IndexingThread::IndexingThread(QObject *parent, QDir dir, bool _topLevelSpecial) : QThread(parent)
{
    parentDir = dir;
    topLevelSpecial = _topLevelSpecial;
}

void IndexingThread::run() {
    isTopLevelSet = false;
    // we scan the top level directory here.
    QTreeWidgetItem *widgetItem = scanDir(parentDir);
    emit resultReady(widgetItem);
}

QTreeWidgetItem* IndexingThread::scanDir(QDir dir) {
    // create a new tree widget item
    QTreeWidgetItem *folder = new QTreeWidgetItem();
    // ui icon stuffs
    if (topLevelSpecial && topLevelSpecial) {
        isTopLevelSet = true;
        folder->setText(0, topLevelText);
        folder->setIcon(0, topLevelIcon);
    } else {
        folder->setIcon(0, folderIcon);
        folder->setText(0, dir.dirName());
    }

    // ensure we are only looking for folders and not "." or ".." as they'll appear as folders too
    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    // get a list of the inner directories
    QStringList dirList = dir.entryList();
    foreach(QString dirName, dirList) {
        // for every directory, in the the current directory, call scanDir() again to get a list of that directory
        // recursively...
        QString newPath = QString("%1/%2").arg(dir.absolutePath()).arg(dirName);
        folder->addChild(scanDir(QDir(newPath)));
    }

    // TODO: make a supported files list so supported files are more centralised.
    dir.setNameFilters(QStringList() << "*.mp3" << "*.wav" << "*.flac");
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    // this recursive code *has* the chance of crashing the thread and causing an overflow error.
    // TODO: make sure the recursive file indexing thread so it doesn't have a chance of crashing out.

    foreach(QString filename, dir.entryList()) {
        // looking through every file in the directory...
        QFileInfo fileInfo(dir.path() + "/" + filename);
        QTreeWidgetItem *audioFile = new QTreeWidgetItem();
        audioFile->setText(0, fileInfo.fileName());
        audioFile->setText(1, fileInfo.filePath());

        QMimeDatabase db;
        QMimeType mime = db.mimeTypeForFile(fileInfo.filePath(), QMimeDatabase::MatchContent);

        audioFile->setIcon(0, samplesIcon);

        folder->addChild(audioFile);
    }

    return folder;
}
