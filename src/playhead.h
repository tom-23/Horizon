#ifndef Playhead_H
#define Playhead_H

#include <QGraphicsItem>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QLine>
#include <QPolygonF>
#include <QVector>
#include <QPointF>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QGraphicsView>
#include <QBasicTimer>

class Playhead : public QGraphicsItem
{

public:
    Playhead(QGraphicsView *_view);
    void setHScaleFactor(int _hScaleFactor);
    void setHeight(int _height);
    int getGridLocation();
    void setGridLocation(float _location);

protected:

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QRectF boundingRect() const override;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:

    int hScaleFactor;
    float gridLocation;
    bool pressed = false;
    QPen pen;
    QGraphicsView *view;
    QLine line;


};

#endif // Playhead_H
