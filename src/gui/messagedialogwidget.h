#ifndef MESSAGEDIALOGWIDGET_H
#define MESSAGEDIALOGWIDGET_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class MessageDialogWidget;
}

class MessageDialogWidget : public QDialog
{
    Q_OBJECT

public:
    explicit MessageDialogWidget(QWidget *parent = nullptr, QString title = "titleLabel", QString message = "messageLabel", QString svgPath = "", bool okOnly = false, bool okCancel = false, bool yesNo = false, bool yesNoCancel = false);
    ~MessageDialogWidget();

private:
    Ui::MessageDialogWidget *ui;
};

#endif // MESSAGEDIALOGWIDGET_H
