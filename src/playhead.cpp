#include "playhead.h"
#include "debug.h"

Playhead::Playhead(QGraphicsView *_view) : QGraphicsItem()
{

    pen = QPen(Qt::white, 1);
    view = _view;
    setAcceptHoverEvents(true);
    this->setAcceptDrops(true);

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(ItemSendsGeometryChanges);

    gridLocation = 1.0f;

}

QRectF Playhead::boundingRect() const
{
    return QRectF(0, 0, 1, this->line.y2());
}


void Playhead::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->setPen(pen);
    line.setP1(QPoint(0, 0));

    painter->drawLine(line);
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

    if (pressed)
    {
        this->setPos(pos.x()-5, y());

    }
    gridLocation = (pos.x() / hScaleFactor) + 1;
    qDebug() << "move" << gridLocation;
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

void Playhead::setHScaleFactor(int _hScaleFactor) {
    hScaleFactor = _hScaleFactor;
    setX((gridLocation - 1) * hScaleFactor);
}

void Playhead::setHeight(int _height) {
    line.setP2(QPoint(line.x1(), _height));
    boundingRect();
}

int Playhead::getGridLocation() {
    return gridLocation;
}

void Playhead::setGridLocation(float _location) {
    gridLocation = _location;
    setX((gridLocation - 1) * hScaleFactor);
    //update();
}

