#include "ghostplayhead.h"
#include <QDebug>

GhostPlayhead::GhostPlayhead(QGraphicsView *_view) : QGraphicsItem()
{
    pen = QPen(Qt::red, 1);
    view = _view;

    //setAcceptHoverEvents(true);
    //this->setAcceptDrops(true);
    //setFlag(QGraphicsItem::ItemIsFocusable);
    //setFlag(ItemSendsGeometryChanges);

    //setFlag(QGraphicsItem::ItemIsMovable);

    isVisible = true;
}


QRectF GhostPlayhead::boundingRect() const
{
    return QRectF(0, 0, 1, this->line.y2());
}

void GhostPlayhead::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
        //if (isVisible == true) {
            painter->setPen(pen);
            line.setP1(QPoint(0, 0));
            painter->drawLine(line);
        //}
}

void GhostPlayhead::hideGhostPlayhead() {
    isVisible = false;
}

void GhostPlayhead::showGhostPlayhead() {
    isVisible = true;

}

void GhostPlayhead::setLocation(int x) {
    setX(x);
    update();
}

void GhostPlayhead::setHeight(int _height) {
    line.setP2(QPoint(line.x1(), _height));
    boundingRect();
}
