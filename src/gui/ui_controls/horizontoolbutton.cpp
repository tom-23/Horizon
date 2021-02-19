#include "horizontoolbutton.h"

HorizonToolButton::HorizonToolButton(QWidget *parent) : QToolButton(parent)
{
    setLEDType(LED_Types::primary);
    connect(this, &HorizonToolButton::toggled, this, &HorizonToolButton::animateChecked);
}

void HorizonToolButton::setColor (QColor _color){
    m_color = _color;
    this->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue()));
}
QColor HorizonToolButton::getColor(){
    return m_color; // getter is not really needed for now
}


void HorizonToolButton::animateChecked(bool checked) {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "bckColor");
    animation->setDuration(300);
    QColor offColor = dialogs::getThemeManager()->getColor("background");;
    QString suffex;
    if (m_ledType == LED_Types::primary) {
        suffex = "primary";
    } else if (m_ledType == LED_Types::secondary) {
        suffex = "secondary";
    } else {
        suffex = "tertiary";
    }
    QColor onColor = dialogs::getThemeManager()->getColor("pushbutton-checked-" + suffex);
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

void HorizonToolButton::paintEvent(QPaintEvent *)
{
    QStylePainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    p.drawComplexControl(QStyle::CC_ToolButton, opt);
}


void HorizonToolButton::setLEDType(LED_Types type) {
    m_ledType = type;
}
