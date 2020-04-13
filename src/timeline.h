#ifndef TIMELINE_H
#define TIMELINE_H

#include <QtGui>
#include <QWidget>
#include <QGraphicsView>
#include <QLayout>
#include <iostream>
#include <vector>

#include "timelinegraphicwidget.h"
#include "rulergraphicwidget.h"
#include "regiongraphicitem.h"
#include "graphicsview.h"
#include "trackcontrolswidget.h"
#include "track.h"



class Timeline
{
public:
    Timeline(QGraphicsView *trackRegions, QGraphicsView *trackRuler, QWidget *trackControls);
    ~Timeline();
    GraphicsView *regionsView;
    GraphicsView *rulerView;

    void setNoteLength(int _noteLength);
    void setBarLength(int _barLength);

    void setBarAmount(int _barAmount);

    void setHZoomFactor(int _hZoomFactor);

    void removeTrack(int _index);
    void addTrack(int _index);


    int noteLength;
    int barLength;
    int barCount;

    int trackCount;

    int hZoomFactor;




private:
    QGraphicsView *trackRegions;
    QGraphicsView *trackRuler;
    QWidget *trackControls;

    TimelineGraphicWidget *timelineGraphic;
    RulerGraphicWidget *rulerGraphic;

    std::vector<class Track *> *trackList;

};

#endif // TIMELINE_H
