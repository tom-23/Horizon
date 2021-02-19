#ifndef HORIZONPUSHBUTTON_H
#define HORIZONPUSHBUTTON_H

#include <QPushButton>
#include <QObject>
#include <QStylePainter>
#include <QStyleOptionButton>
#include <QDebug>
#include <QPropertyAnimation>
#include <QColor>
#include "common/dialogs.h"

class HorizonPushButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor bckColor READ getColor WRITE setColor)
public:
    HorizonPushButton(QWidget *parent = nullptr);
    void setColor (QColor color);
    QColor getColor();
protected:
    virtual void paintEvent(QPaintEvent *) override;

private:
    QColor m_color;
    void animateChecked(bool checked);

};

#endif // HORIZONPUSHBUTTON_H
