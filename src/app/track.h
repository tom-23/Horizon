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
#include <QColor>
#include <iostream>


#include <vector>
#include <memory>
#include <cstdio>
#include <fstream>
#include <cassert>
#include <functional>
#include <math.h>
#include "LabSound/LabSound.h"

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
    Track(Timeline &_timeLine, AudioManager &_audioMan);
    ~Track();

    void setSelected(bool _selected);
    bool getSelected();

    void setTrackControlsWidget(TrackControlsWidget *_tcw);
    void setTrackGraphicsItem(TrackGraphicItem *_tgi);

    QColor getColor();
    void setColor(QColor _color);

    // void setColorTheme(QColor primaryColor);
    void setHScaleFactor(int _hScaleFactor);

    AudioRegion* addAudioRegion();
    void setRegion(Region *_region);
    void removeRegion(Region *_region, Track *newTrack);

    int getIndex();
    void setIndex(int _index);

    AudioRegion* getAudioRegionByIndex(int index);
    int getAudioRegionListCount();


    int getIndexByRegion(Region *region);

    void removeRegion(Region *_region);

    void scheduleAudioRegions();
    void cancelAudioRegions();

    std::shared_ptr<GainNode> getTrackInputNode();
    std::shared_ptr<GainNode> getTrackOutputNode();




    AudioManager* getAudioManager();

    void setMute(bool _mute);
    void setSolo(bool _solo);

    bool getMute();
    bool getSolo();

    void setGain(float _value);
    float getGain();

    std::vector<int> getLMeterData();
    std::vector<int> getRMeterData();

    float peakdB;



private:
    bool selected;
    int index;


    std::vector<class Region *> *regionList;

    std::shared_ptr<GainNode> trackInputNode;
    std::shared_ptr<GainNode> trackOutputNode;

    std::shared_ptr<AnalyserNode> Lanalyser;
    std::shared_ptr<AnalyserNode> Ranalyser;

    std::shared_ptr<ChannelSplitterNode> channelSplitter;
    std::shared_ptr<ChannelMergerNode> channelMerger;

    AudioManager *audioMan;

    Timeline *timeline;

    TrackControlsWidget *trackControlWidget;
    TrackGraphicItem *trackGraphicItem;

    QColor color;

    bool mute;
    bool solo;

    float gain;

};

#endif // TRACK_H
