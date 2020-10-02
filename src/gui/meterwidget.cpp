#include "meterwidget.h"

MeterWidget::MeterWidget(QGraphicsView *_view, int _min, int _max, bool _stereo)
{
    debug::out(3, "Meter Init");
    view = _view;

    QLinearGradient RMSgradient(0, 0, view->width(), 0);

    RMSgradient.setColorAt(0, QColor("#00ff1e"));
    RMSgradient.setColorAt(0.75, QColor("#fbff00"));
    RMSgradient.setColorAt(1, QColor("#ff0000"));

    RMSbrush = QBrush(RMSgradient);

    QLinearGradient Pwrgradient(0, 0, view->width(), 0);

    Pwrgradient.setColorAt(0, QColor("#8000ff1e"));
    Pwrgradient.setColorAt(0.75, QColor("#80fbff00"));
    Pwrgradient.setColorAt(1, QColor("#80ff0000"));

    Pwrbrush = QBrush(Pwrgradient);

    pen = QPen(Qt::transparent, 1);

    min = _min;
    max = _max;

    stereo = _stereo;


}

void MeterWidget::setRMSValue(int _ch1value, int _ch2value) {
    ch1RMSValue = _ch1value;
    ch2RMSValue = _ch2value;
}

void MeterWidget::setPwrValue(int _ch1value, int _ch2value) {
    ch1PwrValue = _ch1value;
    ch2PwrValue = _ch2value;
}

QRectF MeterWidget::boundingRect() const
{
    return QRectF(0, 0, view->width(), view->height());
}

void MeterWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    painter->setPen(pen);

    int ch1Pwrwidth = round(view->width() * (float(ch1PwrValue) / max));
    int ch1RMSwidth = round(view->width() * (float(ch1RMSValue) / max));

    if (!stereo) {

        ch1Pwrmeter = QRect(0, 0, ch1Pwrwidth, boundingRect().height());
        ch1RMSmeter = QRect(0, 0, ch1RMSwidth, boundingRect().height());

    } else {

        int ch2Pwrwidth = round(view->width() * (float(ch2PwrValue) / max));
        int ch2RMSwidth = round(view->width() * (float(ch2RMSValue) / max));

        ch1Pwrmeter = QRect(0, 0, ch1Pwrwidth, (boundingRect().height() / 2) - 1);
        ch2Pwrmeter = QRect(0, (boundingRect().height() / 2) + 1, ch2Pwrwidth, boundingRect().height());

        ch1RMSmeter = QRect(0, 0, ch1RMSwidth, (boundingRect().height() / 2) - 1);
        ch2RMSmeter = QRect(0, (boundingRect().height() / 2) + 1, ch2RMSwidth, boundingRect().height());


        painter->setBrush(Pwrbrush);
        painter->drawRect(ch2Pwrmeter);

        painter->setBrush(RMSbrush);
        painter->drawRect(ch2RMSmeter);
    }

    painter->setBrush(Pwrbrush);
    painter->drawRect(ch1Pwrmeter);

    painter->setBrush(RMSbrush);
    painter->drawRect(ch1RMSmeter);
    //qDebug() << "UPDATING";




}
