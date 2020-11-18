#ifndef UDSTATUSWINDOW_H
#define UDSTATUSWINDOW_H

#include <QDialog>
#include "common/dialogs.h"

namespace Ui {
class UDStatusWindow;
}

class UDStatusWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UDStatusWindow(QWidget *parent = nullptr, QList<QString> listOfItems = {});
    ~UDStatusWindow();
    void popTop();
    void updateProgress(int value);
    void updateMax(int value);

private:
    Ui::UDStatusWindow *ui;
};

#endif // UDSTATUSWINDOW_H
