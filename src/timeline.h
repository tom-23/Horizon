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

#include "timelinegraphicwidget.h"
#include "rulergraphicwidget.h"

#include "graphicsscene.h"
#include "graphicsview.h"
#include "trackcontrolswidget.h"
#include "trackgraphicitem.h"
#include "track.h"

#include "region.h"
#include "thememanager.h"
#include "playhead.h"
#include "ghostplayhead.h"

class GraphicsView;
class Track;
class Region;

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

    int getTrackCount();


    void addRegion(Region *_region);
    void removeRegion(Region *_region);
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

    GhostPlayhead *ghostPlayheadGraphic;


private:
    GraphicsView *trackRegions;
    GraphicsScene *trackRegionsScene;
    QGraphicsView *trackRuler;
    QScrollArea *trackControls;

    QColor primaryColor;

    TimelineGraphicWidget *timelineGraphic;
    RulerGraphicWidget *rulerGraphic;
    Playhead *playheadGraphic;


    std::vector<class TrackControlsWidget *> *trackWidgetList;

    int trackCount;
    int regionCount;

    void setZRegionValues(int _zValue);

    QWidget *parent;

};

#endif // TIMELINE_H
