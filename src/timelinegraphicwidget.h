#ifndef TimelineGraphicWidget_H
#define TimelineGraphicWidget_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTransform>
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QPen>
#include <QBrush>
#include <QGraphicsItem>
#include <QPoint>
#include <iostream>

#include "playhead.h"
#include "regiongraphicitem.h"


using namespace std;

class TimelineGraphicWidget : public QWidget
{
    Q_OBJECT

public:
    TimelineGraphicWidget(QGraphicsView *_view, QWidget *_parent = nullptr);
    ~TimelineGraphicWidget();
    QGraphicsItem *ItemAt(QPointF position) { return scene->itemAt(position, QTransform()); }
    void SetFrame(int _frame)
    {
        if (_frame < maxFrame)
            frame = _frame;
    }
    QGraphicsScene *scene;
    QGraphicsView *view;
    //    QPoint indicatorPos;
    QGraphicsItem *indicatorItem;
    QGraphicsItem *indicatorHead;
    Playhead *indicator;

    int frame;
    int minFrame, maxFrame;


    void setHScaleFactor(int _value);
    void setBarAmount(int _value);
    void setBarLength(int _value);



private:
    QList<QList<QGraphicsLineItem*>*> *barLines;
    int hScaleFactor;
    int vScaleFactor;
    int barAmount;
    int barLength;

    void mousePressEvent(QMouseEvent *event);

protected:
    void resizeEvent(QResizeEvent *event);

};

#endif // TimelineGraphicWidget_H
