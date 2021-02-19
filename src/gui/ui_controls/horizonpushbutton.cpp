#include "horizonpushbutton.h"

HorizonPushButton::HorizonPushButton(QWidget *parent) : QPushButton(parent)
{
    connect(this, &HorizonPushButton::toggled, this, &HorizonPushButton::animateChecked);
}

void HorizonPushButton::setColor (QColor _color){
    m_color = _color;
    this->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue()));
}
QColor HorizonPushButton::getColor(){
    return m_color; // getter is not really needed for now
}

void HorizonPushButton::animateChecked(bool checked) {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "bckColor");
    animation->setDuration(300);
    QColor offColor = dialogs::getThemeManager()->getColor("pushbutton-background");
    QColor onColor = dialogs::getThemeManager()->getColor("pushbutton-checked-primary");
    if (checked) {
        animation->setStartValue(offColor);
        animation->setEndValue(onColor);
    } else {
        animation->setStartValue(onColor);
        animation->setEndValue(offColor);
    }

    animation->setEasingCurve(QEasingCurve::InOutExpo);
    animation->start();
}

void HorizonPushButton::paintEvent(QPaintEvent *)
{
    QStylePainter p(this);
    QStyleOptionButton option;
    initStyleOption(&option);
    p.drawControl(QStyle::CE_PushButton, option);
}
