#include "playhead.h"

Playhead::Playhead(float height) : QGraphicsItem()
{
    pen = QPen(Qt::white, 1);
    brush = QBrush(Qt::RoundCap);
    QColor brushColor = QColor("#999999");
    brushColor.setAlpha(200);
    brush.setColor(brushColor);

    points << QPointF(0, 0)
           << QPointF(0, 6)
           << QPointF(6, 12)
           << QPointF(12, 6)
           << QPointF(12, 0);
    setHeight(height);
    setAcceptHoverEvents(true);
    this->setAcceptDrops(true);

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(ItemSendsGeometryChanges);
    //    setFlags(ItemIsMovable);

    //line.setLine(0,0,0,);
}

QSizeF Playhead::calculateSize() const
{
    float minX = points[0].x();
    float minY = points[0].y();
    float maxX = points[0].x();
    float maxY = points[0].y();
    for (QPointF point : points)
    {
        if (point.x() < minX)
        {
            minX = point.x();
        }
        if (point.y() < minY)
        {
            minY = point.y();
        }
        if (point.x() > maxX)
        {
            maxX = point.x();
        }
        if (point.y() > maxY)
        {
            maxY = point.y();
        }
    }
    return QSizeF(maxX - minX, line.p2().y());
}

QRectF Playhead::boundingRect() const
{
    QSizeF size = this->calculateSize();
    return QRectF(0, 0, size.width(), size.height());
}

void Playhead::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->setPen(pen);
    line.setP1(QPoint(6, 0));
    painter->drawLine(line);
    painter->setBrush(brush);
    //painter->translate(-boundingRect().width()/2,0);
    painter->drawPolygon(points);
    //painter->translate(boundingRect().width()/2,0);
    //    QBrush b(Qt::red);
    //    painter->setBrush(b);
    //    painter->drawRect(boundingRect());
}

void Playhead::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        pressed = true;
        qDebug() << "press";
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        pressed = false;
        qDebug() << "release";
    }

    QGraphicsItem::mousePressEvent(event);
    update();
}

void Playhead::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->scenePos();
    qDebug() << "move";
    if (pressed)
    {

        this->setPos(pos.x(), y());
    }
    QGraphicsItem::mouseMoveEvent(event);
    update();
}

void Playhead::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    qDebug() << event->scenePos();
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void Playhead::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug()<<"hover enter";
}

void Playhead::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug()<<"hover move";

    //    if(pressed){

    //        QPointF pos = event->scenePos();
    //        qDebug()<<pos;
    //        this->setPos(pos.x(),y());
    //    }
}

void Playhead::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug()<<"hover leave";
    //    pressed=false;
}

QVariant Playhead::itemChange(GraphicsItemChange change, const QVariant &value)
{

    if (change == ItemPositionChange && scene())
    {
        // value is the new position.
        QPointF newPos = value.toPointF();
        newPos.setY(y());
        if (newPos.x() < 0)
        {
            newPos.setX(0);
        }
        //        newPos = qBound(QPointF(0,0),newPos,QPointF(10000,0));
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}
