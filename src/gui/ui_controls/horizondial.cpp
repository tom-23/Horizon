#include "horizondial.h"

#include <QPainter>
#include <QColor>

#include <cmath>

HorizonDial::HorizonDial(QWidget* parent,
                       double knobRadius,
                       double knobMargin)
: QDial(parent),
  knobRadius_(knobRadius),
  knobMargin_(knobMargin)
{
    // Default range

}

void HorizonDial::setKnobRadius(double radius)
{
    knobRadius_ = radius;
}

double HorizonDial::getKnobRadius() const
{
    return knobRadius_;
}

void HorizonDial::setKnobMargin(double margin)
{
    knobMargin_ = margin;
}

double HorizonDial::getKnobMargin() const
{
    return knobMargin_;
}

QPixmap HorizonDial::getTexture() const {
    return texture_;
}

void HorizonDial::setTexture(QPixmap texture) {
    texture_ = texture;
}


float map(float n, float start1, float stop1, float start2, float stop2) {
    return ((n-start1)/(stop1-start1))*(stop2-start2)+start2;
}

void HorizonDial::paintEvent(QPaintEvent*)
{
    static const double degree270 = 1.5 * M_PI;

    static const double degree225 = 1.25 * M_PI;

    QPainter painter(this);

    // So that we can use the background color
    painter.setBackgroundMode(Qt::TransparentMode);

    // Smooth out the circle
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Use background color
    QColor pointColor(painter.pen().color());
    painter.setBrush(QBrush(pointColor));


    // Store color from stylesheet, pen will be overriden


    // No border
    painter.setPen(QPen(Qt::NoPen));


    // Get ratio between current value and maximum to calculate angle
    double ratio = double(QDial::value()) / QDial::maximum();

    // The maximum amount of degrees is 270, offset by 225
    double angle = ratio * degree270 - degree225;

    float angleDeg = map(QDial::value(), QDial::minimum(), QDial::maximum(), 45, 345);

    // Draw first circle

    painter.translate((QDial::width() / 2.0), (QDial::height() / 2.0));

    qDebug() << angleDeg;
    painter.rotate(angleDeg);


    QPainterPath path(QPointF(-(QDial::height() / 2.0),-(QDial::height() / 2.0)));
    //your mask - ellipse
    path.addEllipse(-(QDial::height() / 2.0),-(QDial::height() / 2.0),QDial::height(), QDial::height());
    painter.setClipPath(path);

    painter.drawImage(-(QDial::height() / 2.0), -(QDial::height() / 2.0), texture_.toImage());

    //painter.drawEllipse(-(QDial::height() / 2.0), -(QDial::height() / 2.0), QDial::height(), QDial::height());

    // Reset color to pointColor from stylesheet



    float center = (QDial::width() / 2.0) - (QDial::height() / 2.0);

    //qDebug() << QString::number(angle);
    //painter.rotate(angle);
    //painter.drawImage(QRectF(center, 0, QDial::height(), QDial::height()), texture_.toImage());


    painter.setBrush(QBrush(Qt::green));
    // Radius of background circle
    double r = QDial::height() / 2.0;

    // Add r to have (0,0) in center of dial
    double y = sin(angle) * (r - knobRadius_ - knobMargin_) + r;

    double x = (cos(angle) * (r - knobRadius_ - knobMargin_) + r) + (QDial::width() / 2.0) - (QDial::height() / 2.0);

    // Draw the ellipse
    painter.drawEllipse(QPointF(x,y),knobRadius_, knobRadius_);
}
