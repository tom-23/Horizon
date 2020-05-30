#ifndef TRACK_H
#define TRACK_H
#include "timeline.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTransform>
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QPen>
#include <QBrush>
#include <QGraphicsItem>
#include <QPoint>
#include <iostream>


class Track : public std::vector<Track>
{
public:
    Track();
    Track(TrackControlsWidget *controlsWidget, TimelineGraphicWidget *timeLineWidget, int _index);
    QGraphicsLineItem* seperator;
    QGraphicsRectItem* selectionRect;

    void setIndex(int _value);
    void setSelected(bool _selected);

private:
    int index;
    bool selected;
};

#endif // TRACK_H
