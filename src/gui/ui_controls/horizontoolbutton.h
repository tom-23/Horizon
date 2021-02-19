#ifndef HORIZONToolBUTTON_H
#define HORIZONToolBUTTON_H

#include <QToolButton>
#include <QObject>
#include <QStylePainter>
#include <QStyleOptionButton>
#include <QDebug>
#include <QPropertyAnimation>
#include <QColor>
#include "common/dialogs.h"

class HorizonToolButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(QColor bckColor READ getColor WRITE setColor)
    Q_PROPERTY(LED_Types led_type MEMBER m_ledType WRITE setLEDType)
public:
    HorizonToolButton(QWidget *parent = nullptr);
    void setColor (QColor color);
    QColor getColor();

    enum LED_Types {
        primary,
        secondary,
        tertiary
    };

    void setLEDType(LED_Types type);
    LED_Types getLEDType();

protected:
    virtual void paintEvent(QPaintEvent *) override;

private:
    QColor m_color;
    void animateChecked(bool checked);

    LED_Types m_ledType;

};

#endif // HORIZONToolBUTTON_H
