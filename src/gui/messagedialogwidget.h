#ifndef MESSAGEDIALOGWIDGET_H
#define MESSAGEDIALOGWIDGET_H

#include <QDialog>

namespace Ui {
class MessageDialogWidget;
}

class MessageDialogWidget : public QDialog
{
    Q_OBJECT

public:
    explicit MessageDialogWidget(QWidget *parent = nullptr);
    ~MessageDialogWidget();

private:
    Ui::MessageDialogWidget *ui;
};

#endif // MESSAGEDIALOGWIDGET_H
