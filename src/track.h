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

#include "region.h"
#include "audioregion.h"
#include "audiomanager.h"
#include "trackgraphicitem.h"

using namespace lab;

class Region;
class AudioRegion;
class AudioManager;
class TrackGraphicItem;
class TrackControlsWidget;

class Track
{
public:
    Track(Timeline *_timeLine, AudioManager *_audioMan);

    void setSelected(bool _selected);
    bool getSelected();

    void setTrackControlsWidget(TrackControlsWidget *_tcw);
    void setTrackGraphicsItem(TrackGraphicItem *_tgi);

    // void setColorTheme(QColor primaryColor);
    void setHScaleFactor(int _hScaleFactor);

    AudioRegion* addAudioRegion();
    void setRegion(Region *_region);

    int getIndex();
    void setIndex(int _index);

    // void removeRegion(Region *_region);

    std::shared_ptr<AudioContext>* getAudioContext();
    std::shared_ptr<GainNode>* getTrackNode();
    AudioManager* getAudioManager();

private:
    bool selected;
    int index;


    std::vector<class Region *> *regionList;

    std::shared_ptr<AudioContext> *context;
    std::shared_ptr<GainNode> *trackNode;
    AudioManager *audioMan;

    Timeline *timeline;

    TrackControlsWidget *trackControlWidget;
    TrackGraphicItem *trackGraphicItem;

};

#endif // TRACK_H
