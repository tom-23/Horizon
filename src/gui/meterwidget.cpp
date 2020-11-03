#include "meterwidget.h"

MeterWidget::MeterWidget(QGraphicsView *_view, int _min, int _max, bool _stereo)
{
    debug::out(3, "Meter Init");
    view = _view;
    vertical = !(view->width() > view->height());





    pen = QPen(Qt::transparent, 0);

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
    int paddingx2 = padding * 2;

    QTransform transform;
    //transform.translate(2, 2);

    vertical = !(view->width() > view->height());



    float movingVal;

    if (!vertical) {
        movingVal = view->width();
    } else {
        movingVal = view->height();
    }

    if (view->height() != lastmv) {
        updateGradients();
        lastmv = round(movingVal);
    }

    //transform.scale(0.5, 0.5);
    //painter->setTransform(transform);

    float ch1PwrWH = movingVal * (float(ch1PwrValue) / max);
    float ch1RMSWH = movingVal * (float(ch1RMSValue) / max);

    if (!stereo) {

        if (!vertical) {

            ch1Pwrmeter = QRectF(0, 0, ch1PwrWH - 4, boundingRect().height() - 4);
            ch1RMSmeter = QRectF(0, 0, ch1RMSWH - 4, boundingRect().height() - 4);
        } else {

        }


    } else {

        float ch2PwrWH = movingVal * (float(ch2PwrValue) / max);
        float ch2RMSWH = movingVal * (float(ch2RMSValue) / max);

        if (!vertical) {

            ch1Pwrmeter = QRectF(0, 0, ch1PwrWH, (boundingRect().height() / 2) - 1);
            ch2Pwrmeter = QRectF(0, (boundingRect().height() / 2) + 1, ch2PwrWH, boundingRect().height());

            ch1RMSmeter = QRectF(0, 0, ch1RMSWH - 2, ((boundingRect().height() / 2) - 1));
            ch2RMSmeter = QRectF(0, ((boundingRect().height() / 2) + 1), ch2RMSWH, ((boundingRect().height() / 2) - 0));
        } else {

            int widthL = ((boundingRect().width() / 2) - 1);
            int widthR = ((boundingRect().width() / 2) + 1);
            ch1RMSmeter = QRectF(0, boundingRect().height(), widthL, (ch1RMSWH - 2) * -1);
            ch2RMSmeter = QRectF(widthR, boundingRect().height() , widthR, (ch2RMSWH - 2) * -1);
            ch1Pwrmeter = QRectF(0, boundingRect().height(), widthL, (ch1PwrWH - 2) * -1);
            ch2Pwrmeter = QRectF(widthR, boundingRect().height() , widthR, (ch2PwrWH - 2) * -1);
        }

        painter->setBrush(RMSbrush);
        painter->drawRect(ch2RMSmeter);

        //painter->setBrush(Pwrbrush);
        //painter->drawRect(ch2Pwrmeter);


    }

    //painter->setBrush(Pwrbrush);
    //painter->drawRect(ch1Pwrmeter);

    painter->setBrush(RMSbrush);
    painter->drawRect(ch1RMSmeter);

}

void MeterWidget::updateGradients() {

    if (!vertical) {
        RMSgradient =  QLinearGradient(0, 0, view->width(), 0);
        Pwrgradient =  QLinearGradient(0, 0, view->width(), 0);
    } else {
        RMSgradient =  QLinearGradient(0, view->height(), 0, 0);
        Pwrgradient =  QLinearGradient(0, view->height(), 0, 0);
    }

    Pwrgradient.setColorAt(0, QColor("#8000ff1e"));
    Pwrgradient.setColorAt(0.75, QColor("#80fbff00"));
    Pwrgradient.setColorAt(1, QColor("#80ff0000"));

    Pwrbrush = QBrush(Pwrgradient);

    RMSgradient.setColorAt(0, QColor("#00ff1e"));
    RMSgradient.setColorAt(0.75, QColor("#fbff00"));
    RMSgradient.setColorAt(1, QColor("#ff0000"));

    RMSbrush = QBrush(RMSgradient);
}
