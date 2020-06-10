#ifndef REGIONGRAPHICITEM_H
#define REGIONGRAPHICITEM_H

#include <QGraphicsItem>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

#include "region.h"

class Timeline;
class Region;


class RegionGraphicItem : public QGraphicsItem
{
public:
    RegionGraphicItem(int _length, QColor _color, QGraphicsScene *_scene, Timeline *_timeline, Region *_region);
    RegionGraphicItem(QGraphicsScene *_scene, QColor _color, Timeline *_timeline, Region *_region);
    float getGridLocation();
    void setGridLocation(float _value);





    // QGraphicsItem interface
protected:
    //    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    // QGraphicsItem interface
protected:

    QColor color;
    QColor outlineColor;
    QColor selectedColor;
    QColor selectedColorOutline;
    int penWidth;
    int rounded;
    bool hasShadow;
    float thresholdShadow;
    QBrush brush;
    QPen pen;
    int length, height;
    bool pressed = false;
    QPointF oldPos, oldMousePos;
    int oldTrackIndex;
    float gridLocation;
    QGraphicsScene *scene;
    Region *region;
    // QGraphicsItem interface

    Timeline *timeline;

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void keyPressEvent(QKeyEvent *event) override;

private:



};

#endif // RegionGraphicItem_H
