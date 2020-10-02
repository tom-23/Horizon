#ifndef METERWIDGET_H
#define METERWIDGET_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <qdebug.h>
#include "common/debug.h"

class MeterWidget : public QGraphicsItem
{
public:
    MeterWidget(QGraphicsView *_view, int min, int max, bool stereo);
    void setRMSValue(int _ch1value, int _ch2value = 0);
    void setPwrValue(int _ch1value, int _ch2value = 0);



protected:
    QGraphicsView *view;

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QBrush RMSbrush;
    QBrush Pwrbrush;
    QPen pen;

    QRect ch1RMSmeter;
    QRect ch2RMSmeter;

    QRect ch1Pwrmeter;
    QRect ch2Pwrmeter;

    int ch1RMSValue;
    int ch2RMSValue;

    int ch1PwrValue;
    int ch2PwrValue;

    int min;
    int max;

    bool stereo;






};

#endif // METERWIDGET_H
