#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include "arrangewidget.h"
#include "common/preferences.h"

class ArrangeWidget;

namespace Ui {
class InfoWidget;
}

class InfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();

private slots:
    void on_toolBox_currentChanged(int index);

private:
    QIcon masterIconSelected;
    QIcon masterIcon;
    QIcon trackIconSelected;
    QIcon trackIcon;
    QIcon regionIconSelected;
    QIcon regionIcon;

    Ui::InfoWidget *ui;
    void resetIcons();
};

#endif // INFOWIDGET_H
