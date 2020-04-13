#ifndef RegionGraphicItem_H
#define RegionGraphicItem_H

#include <QGraphicsItem>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

class RegionGraphicItem : public QGraphicsItem
{
public:
    RegionGraphicItem(QGraphicsScene *_scene, QColor _color) : QGraphicsItem()
    {
        setFlags(ItemIsMovable);
        color = _color;
        outlineColor = color.lighter(30);
        selectedColor = QColor(200,30,180);
        selectedColorOutline = selectedColor.lighter(30);
        //        rounded = 3;
        //        hasShadow = false;
        //        thresholdShadow=0.0f;
        brush = QBrush(color);
        pen = QPen(outlineColor, 1);
        pen.setCapStyle(Qt::RoundCap);
        length = 5;
        height = 60;
        oldPos = pos();
        scene = _scene;
    }
    RegionGraphicItem(int _length, QColor _color, QGraphicsScene *_scene)
    {
        setFlags(ItemIsMovable);
        color = _color;
        outlineColor = color.lighter(30);
        selectedColor = QColor(200,30,180);
        selectedColorOutline = selectedColor.lighter(30);
        //        rounded = 3;
        //        hasShadow = false;
        //        thresholdShadow=0.0f;
        brush = QBrush(color);
        pen = QPen(outlineColor, penWidth);
        pen.setCapStyle(Qt::RoundCap);
        length = _length;
        height = 60;
        oldPos = scenePos();
        scene = _scene;
    }

    void SetLength(float _length)
    {
        length = _length;
    }

    QColor color;
    QColor outlineColor;
    static QColor selectedColor;
    static QColor selectedColorOutline;
    static int penWidth;
    static int rounded;
    static bool hasShadow;
    static float thresholdShadow;
    QBrush brush;
    QPen pen;
    int length, height;
    bool pressed = false;
    QPointF oldPos, oldMousePos;
    QGraphicsScene *scene;
    // QGraphicsItem interface

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // QGraphicsItem interface
protected:
    //    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    // QGraphicsItem interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:



};

#endif // RegionGraphicItem_H
