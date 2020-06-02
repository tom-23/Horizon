#ifndef TIMELINECONTROL_H
#define TIMELINECONTROL_H

#include <QtGui>
#include <QWidget>
#include <QGraphicsView>
#include <QLayout>
#include <iostream>
#include <vector>

#include "timelinegraphicwidget.h"
#include "rulergraphicwidget.h"

#include "graphicsview.h"

#include "trackcontrolswidget.h"
#include "track.h"

#include "region.h"
#include "thememanager.h"




class Timeline
{
public:
    Timeline(QGraphicsView *trackRegions, QGraphicsView *trackRuler, QWidget *trackControls);
    ~Timeline();
    GraphicsView *regionsView;
    GraphicsView *rulerView;

    void setColorTheme(ThemeManager *_themeManager);


    void setNoteLength(int _noteLength);
    void setBarLength(int _barLength);

    void setBarAmount(int _barAmount);

    void setHZoomFactor(int _hZoomFactor);
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



    int hZoomFactor;


    QString backgroundColor;
    QString linesColor;
    QString playheadColor;
    QString selectBoxColor;
    QString textColor;


private:
    QGraphicsView *trackRegions;
    QGraphicsView *trackRuler;
    QWidget *trackControls;

    TimelineGraphicWidget *timelineGraphic;
    RulerGraphicWidget *rulerGraphic;

    std::vector<class Track *> *trackList;
    std::vector<class Region *> *regionList;

    int trackCount;
    int regionCount;

    void setZRegionValues(int _zValue);

};

#endif // TIMELINE_H
