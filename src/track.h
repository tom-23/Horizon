#ifndef TRACK_H
#define TRACK_H

//#include "timeline.h"
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

//#include "audioregion.h"
//class Region;
//class AudioRegion;

class AudioManager;
class Region;
#include "audiomanager.h"

class AudioRegion;

class TrackControlsWidget;
class TrackGraphicItem;
class Timeline;

//#include "trackcontrolswidget.h"
//#include "trackgraphicitem.h"

//#include "LabSound/LabSound.h"

using namespace lab;

class Track
{
public:
    Track(Timeline *_timeLine, AudioManager *_audioMan);
    ~Track();

    void setSelected(bool _selected);
    bool getSelected();

    void setTrackControlsWidget(TrackControlsWidget *_tcw);
    void setTrackGraphicsItem(TrackGraphicItem *_tgi);

    // void setColorTheme(QColor primaryColor);
    void setHScaleFactor(int _hScaleFactor);

    AudioRegion* addAudioRegion();
    void setRegion(Region *_region);
    void switchRegion(Region *_region, Track *newTrack);

    int getIndex();
    void setIndex(int _index);

    int getIndexByRegion(Region *region);

    void removeRegion(Region *_region);

    void scheduleAudioRegions();
    void cancelAudioRegions();

    std::shared_ptr<AudioContext> getAudioContext();
    std::shared_ptr<GainNode> getTrackInputNode();
    std::shared_ptr<GainNode> getTrackOutputNode();

    AudioManager* getAudioManager();

private:
    bool selected;
    int index;


    std::vector<class Region *> *regionList;

    std::shared_ptr<AudioContext> context;
    std::shared_ptr<GainNode> trackInputNode;
    std::shared_ptr<GainNode> trackOutputNode;

    AudioManager *audioMan;

    Timeline *timeline;

    TrackControlsWidget *trackControlWidget;
    TrackGraphicItem *trackGraphicItem;

};

#endif // TRACK_H
