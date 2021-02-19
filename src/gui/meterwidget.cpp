#include "meterwidget.h"

MeterWidget::MeterWidget(QGraphicsView *_view, int _min, int _max, QColor _borderColor)
{
    debug::out(3, "Meter Init");
    view = _view;
    setSize(view->width(), view->height());
    vertical = !(width > height);
    pen = QPen(Qt::transparent, 0);

    min = _min;
    max = _max;

    borderColor = _borderColor;
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
    return QRectF(0, 0, width, height);
}

void MeterWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    // this is bad dumb code. never write code like this. this needs to get fixed.
    // TODO: reimplement meters so they are not shit.

    int paddingx2 = padding * 2;

    QTransform transform;

    vertical = !(width > height);

    float movingVal;

    if (!vertical) {
        movingVal = width;
    } else {
        movingVal = height;
    }

    if (height != lastmv) {
        updateGradients();
        lastmv = round(movingVal);
    }

    float zerodB = movingVal * (float(100) / max);

    painter->setPen(QPen(borderColor, 1));

    if (!vertical) {
        painter->drawLine(zerodB, 0, zerodB, height);
    } else {
        QFont font = scene()->font();
        font.setPixelSize(9);
        font.setBold(true);
        painter->setFont(font);
        painter->drawText((width / 2) - 3, (height - zerodB) + 3, "0");
        int lineHeight = height - zerodB;
        painter->drawLine(0, lineHeight, (width / 2) - 5, lineHeight);
        painter->drawLine((width / 2) + 5, lineHeight, width, lineHeight);
    }
    painter->setPen(QPen(Qt::transparent, 0));

    //transform.scale(0.5, 0.5);
    //painter->setTransform(transform);

    float ch1PwrWH = movingVal * (float(ch1PwrValue) / max);
    float ch1RMSWH = movingVal * (float(ch1RMSValue) / max);

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

    //painter->setBrush(Pwrbrush);
    //painter->drawRect(ch1Pwrmeter);



    painter->setBrush(RMSbrush);
    painter->drawRect(ch1RMSmeter);

}

void MeterWidget::updateGradients() {

    if (!vertical) {
        RMSgradient =  QLinearGradient(0, 0, width, 0);
        Pwrgradient =  QLinearGradient(0, 0, width, 0);
    } else {
        RMSgradient =  QLinearGradient(0, height, 0, 0);
        Pwrgradient =  QLinearGradient(0, height, 0, 0);
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

void MeterWidget::setSize(int _width, int _height) {
    // this is utterly fucking retarded.
    if (width != _width || height != _height) {
        //prepareGeometryChange();
        width = _width;
        height = _height;
    }
}
