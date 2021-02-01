#include "trackgraphicitem.h"

TrackGraphicItem::TrackGraphicItem(QGraphicsScene *_scene, Timeline *_timeline, Track *_track) : QGraphicsItem()
{
    scene = _scene;
    timeline = _timeline;
    track = _track;
    length = scene->sceneRect().width();
    yValue = (track->getIndex() + 1) * 60;
    setY(yValue);
}

void TrackGraphicItem::updateLength() {
    prepareGeometryChange();
    if ((timeline->barCount * timeline->hZoomFactor) < scene->views()[0]->width()) {
        length = scene->views()[0]->width();
    } else {
        length = (timeline->barCount * timeline->hZoomFactor);
    }
}

void TrackGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(option);
    painter->setPen(QPen(timeline->getPrimaryColor(), 1));
    painter->drawLine(QLine(0, 0,boundingRect().width(), 0));


}

QRectF TrackGraphicItem::boundingRect() const {
    return QRectF(0, 0, length, 1);
}


