#ifndef TimelineGraphicWidget_H
#define TimelineGraphicWidget_H

#include <QWidget>
#include <QObject>
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
#include <QGraphicsItemGroup>
#include <iostream>

#include "graphicsview.h"


using namespace std;

class TimelineGraphicWidget : public QWidget
{
    Q_OBJECT

public:
    TimelineGraphicWidget(QGraphicsView *_view, QWidget *_parent = nullptr, int _barAmount = 8, int _barLength = 4);
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



    int frame;
    int minFrame, maxFrame;


    void setHScaleFactor(int _value);
    void setHeights(int _value);
    void setBarAmount(int _value);
    void setBarLength(int _value);

    int getBarAmount();

    void setColorTheme(QColor primaryLinesColor, QColor secondaryLinesColor);



private:
    QList<QList<QGraphicsLineItem*>*> *barLines;

    int hScaleFactor;
    int vScaleFactor;
    int barAmount;
    int barLength;



    QColor primaryColor;
    QColor seconardColor;

    QGraphicsRectItem *viewportPadding;

 protected:
    virtual void resizeEventSlot(QResizeEvent *event);

    virtual void dragEnterEventSlot(QDragEnterEvent *event);

    bool eventFilter(QObject *object, QEvent *event);
};

#endif // TimelineGraphicWidget_H
