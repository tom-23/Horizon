#ifndef LIBRARYWIDGET_H
#define LIBRARYWIDGET_H

#include <QWidget>
#include "common/preferences.h"
#include "app/indexingthread.h"
#include "arrangewidget.h"

#include <QTreeWidgetItem>

class ArrangeWidget;

namespace Ui {
class LibraryWidget;
}

class LibraryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LibraryWidget(QWidget *parent = nullptr, Preferences *prefs = nullptr, ArrangeWidget *arrangeWidget = nullptr);
    ~LibraryWidget();

    void refesh();
    void refreshFactoryContent();

private slots:
    void on_refeshButton_clicked();

    void on_searchBox_textChanged(const QString &arg1);

    void on_libraryTree_itemDoubleClicked(QTreeWidgetItem *item, int column);
private:
    Ui::LibraryWidget *ui;
    Preferences *prefs;

    QIcon folderIcon;
    QIcon samplesIcon;

    int itemsToIndex;

    void indexComplete(QTreeWidgetItem *parentItem);

    void showParent(QTreeWidgetItem *item);

    ArrangeWidget *arrangeWidget;

    QString bundleLoc;

    QList<QList<QString>> factoryContent;

};

#endif // LIBRARYWIDGET_H
