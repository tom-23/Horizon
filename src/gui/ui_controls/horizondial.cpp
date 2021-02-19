#include "horizondial.h"

#include <QPainter>
#include <QColor>
#include <QLabel>
#include <QRectF>
#include <QPen>
#include <QResizeEvent>

#include <cmath>

HorizonDial::HorizonDial(const QString& suffix,
                         QWidget* parent,
                         int minimum,
                         int maximum)
: QDial(parent),
  suffix_(suffix),
  arcRect_(new QRectF),
  valueRect_(new QRectF),
  suffixRect_(new QRectF),
  arcColor_(new QColor),
  arcPen_(new QPen)
{
    QDial::setRange(minimum, maximum);

    QDial::setCursor(Qt::PointingHandCursor);

        connect(this, &QDial::valueChanged,
                this, &HorizonDial::updateValue);

        setMinimumSize(100,100);

        setMaximumAngle(-360);

        setStartAngle(270);

        updateValue();

}

void HorizonDial::paintEvent(QPaintEvent*)
{
    QPainter painter(this);


        // So that we can use the background color
        // Otherwise the background is transparent
        painter.setBackgroundMode(Qt::OpaqueMode);

        // Smooth out the circle
        painter.setRenderHint(QPainter::Antialiasing);

        // Use background color
        painter.setBrush(painter.background());

        // Get current pen before resetting so we have
        // access to the color() method which returns the
        // color from the stylesheet
        QPen textPen = painter.pen();

        // No border
        painter.setPen(QPen(Qt::NoPen));

        // Draw background circle
        painter.drawEllipse(QDial::rect());

        painter.setPen(textPen);

        painter.drawText(*suffixRect_, Qt::AlignHCenter | Qt::AlignBottom, suffix_);

        painter.drawText(*valueRect_, Qt::AlignCenter, valueString_);

        painter.setPen(*arcPen_);

        painter.drawArc(*arcRect_, startAngle_, angleSpan_);
}

void HorizonDial::updateSize() {
    double width = QDial::width() - (2 * arcWidth_);

    double height = width / 2;

    *suffixRect_ = QRectF(arcWidth_, arcWidth_, width, height);

    *valueRect_ = QRectF(arcWidth_, height, width, height);

    *arcRect_ = QRectF(arcWidth_ / 2,
                       arcWidth_ / 2,
                       QDial::width() - arcWidth_,
                       QDial::height() - arcWidth_);
}

void HorizonDial::resizeEvent(QResizeEvent* event)
{
    //QDial::setMinimumSize(event->size());
    updateSize();
}

void HorizonDial::updateValue()
{
    double value = QDial::value();

    // Get ratio between current value and maximum to calculate angle
    double ratio = value / QDial::maximum();

    angleSpan_ = maximumAngleSpan_ * ratio;

    valueString_ = QString::number(value);
}

void HorizonDial::setArcWidth(double px)
{
    arcWidth_ = px;

    *arcRect_ = QRectF(arcWidth_ / 2,
                       arcWidth_ / 2,
                       QDial::width() - arcWidth_,
                       QDial::height() - arcWidth_);

    arcPen_->setWidth(arcWidth_);
}

void HorizonDial::setSuffix(const QString& text)
{
    suffix_ = text;
}

QString HorizonDial::getSuffix() const
{
    return suffix_;
}

double HorizonDial::getArcWidth() const
{
    return arcWidth_;
}

void HorizonDial::setMaximumAngle(double angle)
{
    maximumAngleSpan_ = angle * 16;
}

double HorizonDial::getMaximumAngle() const
{
    return maximumAngleSpan_ / 16;
}

void HorizonDial::setStartAngle(double angle)
{
    startAngle_ = angle * 16;
}

double HorizonDial::getStartAngle() const
{
    return startAngle_ / 16;
}

void HorizonDial::setArcColor(const QString& color)
{
    arcColor_->setNamedColor(color);

    arcPen_->setColor(*arcColor_);
}

QString HorizonDial::getArcColor() const
{
    return arcColor_->name();
}
