#ifndef HORIZONMETER_H
#define HORIZONMETER_H

#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QDebug>
#include <QResizeEvent>
#include "common/dialogs.h"

class HorizonMeter : public QWidget
{
    Q_OBJECT
public:
    explicit HorizonMeter(QWidget *parent = nullptr);

    void setRMSValue(int _ch1value, int _ch2value = 0);
    void setPwrValue(int _ch1value, int _ch2value = 0);

    void setMinMax(int _min, int _max) {min = _min; max = _max;};

private:
    virtual void paintEvent(QPaintEvent*) override;
    virtual void resizeEvent(QResizeEvent* event) override;


    QLinearGradient Pwrgradient;
    QLinearGradient RMSgradient;

    QBrush RMSbrush;
    QBrush Pwrbrush;
    QPen pen;

    QRectF ch1RMSmeter;
    QRectF ch2RMSmeter;

    QRectF ch1Pwrmeter;
    QRectF ch2Pwrmeter;

    int ch1RMSValue;
    int ch2RMSValue;

    int ch1PwrValue;
    int ch2PwrValue;

    int min;
    int max;

    QColor borderColor;

    bool vertical;

    int padding;


    int lastmv = 0;



    void updateGradients();

    int width = 0;
    int height = 0;

signals:

};

#endif // HORIZONMETER_H
