#ifndef TIMELINECONTROL_H
#define TIMELINECONTROL_H

#include <QtGui>
#include <QWidget>
#include <QGraphicsView>
#include <QLayout>
#include <QSlider>
#include <iostream>
#include <vector>

#include "timelinegraphicwidget.h"
#include "rulergraphicwidget.h"


#include "graphicsscene.h"
#include "graphicsview.h"
#include "trackcontrolswidget.h"
#include "track.h"

#include "region.h"
#include "thememanager.h"


class GraphicsView;

class Timeline
{
public:
    Timeline(QWidget *_parent,
             QGraphicsView *trackRuler,
             QScrollArea *trackControls,
             QLayout *_mainLayout,
             QScrollBar *hScrollBar,
             QScrollBar *vScollBar);
    ~Timeline();

    void setColorTheme(ThemeManager *_themeManager);


    void setNoteLength(int _noteLength);
    void setBarLength(int _barLength);

    void setBarAmount(int _barAmount);

    void setHZoomFactor(int _hZoomFactor, QSlider *zoomSlider = nullptr);
    void updateViewports();

    void addTrack(int _index);
    void removeTrack(int _index);

    int getTrackCount();


    void addRegion(int _trackIndex);
    void removeRegion(int _index);
    void setRegionTrack(int _oldTrackIndex, int _newTrackIndex);

    int getRegionCount();


    int noteLength;
    int barLength;
    int barCount;

    bool pinchToZoom;

    int hZoomFactor;


    QString backgroundColor;
    QString linesColor;
    QString playheadColor;
    QString selectBoxColor;
    QString textColor;


private:
    GraphicsView *trackRegions;
    QGraphicsView *trackRuler;
    QScrollArea *trackControls;

    TimelineGraphicWidget *timelineGraphic;
    RulerGraphicWidget *rulerGraphic;

    std::vector<class Track *> *trackList;
    std::vector<class Region *> *regionList;

    int trackCount;
    int regionCount;



    void setZRegionValues(int _zValue);

    QWidget *parent;

};

#endif // TIMELINE_H
