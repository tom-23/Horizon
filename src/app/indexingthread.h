#ifndef INDEXINGTHREAD_H
#define INDEXINGTHREAD_H

#include <QObject>
#include <QThread>
#include <QDir>
#include <QTreeWidgetItem>
#include <QMimeDatabase>


class IndexingThread : public QThread
{
    Q_OBJECT
public:
    explicit IndexingThread(QObject *parent = nullptr, QDir dir = QDir(), bool topLevelSpecial = false);
    void run() override;
    QTreeWidgetItem* scanDir(QDir dir);

    QString wavIcon;
    QString mp3Icon;
    QString folderIcon;
    QString samplesIcon;

    QDir parentDir;



signals:
    void resultReady(QTreeWidgetItem *treeWidgetItem);
private:
    bool topLevelSpecial;
    bool isTopLevelSet;
};

#endif // INDEXINGTHREAD_H
