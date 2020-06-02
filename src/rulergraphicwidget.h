#ifndef RulerGraphicWidget_H
#define RulerGraphicWidget_H


#include <QGraphicsItem>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPoint>

#include <iostream>

using namespace std;

class RulerGraphicWidget : public QWidget
{
public:
    RulerGraphicWidget(QGraphicsView *_view, QWidget *_parent = nullptr, int _barAmount = 8);
    ~RulerGraphicWidget();
    QGraphicsScene *scene;
    QGraphicsView *view;



    void setHScaleFactor(int _value);
    void setBarAmount(int _value);

    void setColorTheme(QColor linesColor, QColor textColor);

private:
    QList<QGraphicsLineItem*> *barLines;
    QList<QGraphicsTextItem*> *barNumbers;
    int hScaleFactor;
    int barAmount;
    QGraphicsRectItem *viewportPadding;
};

#endif // RulerGraphicWidget_H
