#ifndef TIMELINECONTROL_H
#define TIMELINECONTROL_H

#include <QtGui>
#include <QWidget>
#include <QGraphicsView>
#include <QLayout>
#include <QSlider>
#include <iostream>
#include <vector>
#include <QTimer>
#include <QGraphicsItem>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QApplication>


#include "graphicsscene.h"

class GraphicsView;
#include "graphicsview.h"

#include "playhead.h"
class RegionGraphicItem;
#include "regiongraphicitem.h"
class RulerGraphicWidget;
#include "rulergraphicwidget.h"
class TrackGraphicItem;
#include "trackgraphicitem.h"
class TrackControlsWidget;
#include "trackcontrolswidget.h"
class TimelineGraphicWidget;
#include "timelinegraphicwidget.h"
class ThemeManager;
#include "common/thememanager.h"
class AudioManager;
class Track;
class Region;
class Debug;
#include "common/debug.h"

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
    QColor getPrimaryColor();

    void setNoteLength(int _noteLength);
    void setBarLength(int _barLength);

    void setBarAmount(int _barAmount);

    void setHZoomFactor(int _hZoomFactor, QSlider *zoomSlider = nullptr);
    QGraphicsScene* getScene();
    void updateHeights();
    void updateViewports();

    void setPlayheadLocation(float _location);

    void addTrack(Track *_track);
    void removeTrack(Track *_track);
    void removeTrackGraphicsItem(TrackGraphicItem *tgi);

    int getTrackCount();

    void addRegion(Region *_region);
    void removeRegion(Region *_region);

    void setRegionTrack(int _oldTrackIndex, int _newTrackIndex);

    int getRegionCount();

    void clearAll();


    int noteLength;
    int barLength;
    int barCount;



    bool pinchToZoom;

    int hZoomFactor;

    bool regionSnapping;


    QString backgroundColor;
    QString linesColor;
    QString playheadColor;
    QString selectBoxColor;
    QString textColor;

private:
    GraphicsView *trackRegions;
    GraphicsScene *trackRegionsScene;
    QGraphicsView *trackRuler;
    QScrollArea *trackControls;

    QColor primaryColor;

    TimelineGraphicWidget *timelineGraphic;
    RulerGraphicWidget *rulerGraphic;
    Playhead *playheadGraphic;

    ThemeManager *themeManager;

    QList<RegionGraphicItem*> regionGraphicList;

    int trackCount;
    int regionCount;

    void setZRegionValues(int _zValue);

    QWidget *parent;

    QTimer *uiTimer;

};

#endif // TIMELINE_H
