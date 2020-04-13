#ifndef EFFECTWIDGET_H
#define EFFECTWIDGET_H

#include <QWidget>

namespace Ui {
class EffectWidget;
}

class EffectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EffectWidget(QWidget *parent = nullptr);
    ~EffectWidget();

private:
    Ui::EffectWidget *ui;
};

#endif // EFFECTWIDGET_H
