#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include "effectwidget.h"

namespace Ui {
class InfoWidget;
}

class InfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InfoWidget(QWidget *parent = nullptr);
    void addEffect(EffectWidget *effectWidget);
    ~InfoWidget();

private:
    Ui::InfoWidget *ui;
};

#endif // INFOWIDGET_H
