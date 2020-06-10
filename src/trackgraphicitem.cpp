#include "trackgraphicitem.h"

TrackGraphicItem::TrackGraphicItem(QGraphicsScene *_scene, Timeline *_timeline, Track *_track) : QGraphicsItem()
{
    scene = _scene;
    timeline = _timeline;
    track = _track;
    length = scene->views()[0]->width();
    yValue = (track->getIndex() + 1) * 60;
}

void TrackGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //Q_UNUSED(widget);
    painter->setPen(QPen(timeline->getPrimaryColor(), 1));
    if ((timeline->barCount * timeline->hZoomFactor) < scene->views()[0]->width()) {
        length = scene->views()[0]->width();
    } else {
        length = (timeline->barCount * timeline->hZoomFactor);
    }
    yValue = (track->getIndex() + 1) * 60;
    painter->drawLine(QLine(0, yValue,length, yValue));


}

QRectF TrackGraphicItem::boundingRect() const {
    return QRectF(0, yValue, length, yValue + 1);
}
