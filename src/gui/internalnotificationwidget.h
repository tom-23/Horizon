#ifndef INTERNALNOTIFICATIONWIDGET_H
#define INTERNALNOTIFICATIONWIDGET_H

#include <QWidget>

namespace Ui {
class InternalNotificationWidget;
}

class InternalNotificationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InternalNotificationWidget(QWidget *parent = nullptr);
    ~InternalNotificationWidget();

private:
    Ui::InternalNotificationWidget *ui;
};

#endif // INTERNALNOTIFICATIONWIDGET_H
