#ifndef TRACKGRAPHICITEM_H
#define TRACKGRAPHICITEM_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPen>
#include <QPainter>
#include <QDebug>

#include "audiomanager.h"

class Track;
class Timeline;


class TrackGraphicItem : public QGraphicsItem
{
public:
    TrackGraphicItem(QGraphicsScene *_scene, Timeline *_timeline, Track *_track);
    Track* getTrack();
    //void setHScaleFactor(int _hScaleFactor);
    //void updatePrimaryColor();
protected:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;



private:
    QGraphicsScene *scene;
    Timeline *timeline;
    Track *track;

    QColor primaryColor;

    QLine line;

    int length;
    int yValue;
};

#endif // TRACKGRAPHICITEM_H
