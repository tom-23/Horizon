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
//#include "audioregion.h"
//class Region;
//class AudioRegion;

class AudioManager;
class Region;
#include "audiomanager.h"
#include "audioeffect.h"

#include "effecttypes.h"

class AudioRegion;

class TrackControlsWidget;
class MixerChannelWidget;
class TrackGraphicItem;
class Timeline;

//#include "trackcontrolswidget.h"
//#include "trackgraphicitem.h"

//#include "LabSound/LabSound.h"

class Track
{
public:
    Track(Timeline &_timeLine, AudioManager &_audioMan, QString uuid);
    ~Track();

    void setSelected(bool _selected);
    bool getSelected();

    void setTrackControlsWidget(TrackControlsWidget *_tcw);
    void setMixerChannelWidget(MixerChannelWidget *_mcw);
    void setTrackGraphicsItem(TrackGraphicItem *_tgi);

    QColor getColor();
    void setColor(QColor _color);
    void updateColor(QColor _color);

    AudioRegion* addAudioRegion(QString regionUUID);
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

    AudioManager* getAudioManager();

    QString getUUID();

    void setMute(bool _mute);
    void setSolo(bool _solo);

    bool getMute();
    bool getSolo();

    void setGain(float _value);
    float getGain();

    void setPan(float _value);
    float getPan();

    float peakdB;

    void uiUpdate();

    Region* getSelectedRegion(int index);
    void setRegionSelected(Region *region, bool selected);



    AudioEffect* addAudioEffect(effectType type, QString uuid = "");
    void showEffectWindow(AudioEffect *effect);

    bool isLSilent();
    bool isRSilent();

    void scheduleNextAudioRegion();
    void scheduleNextBar();

    QPair<int, int> lMeterData;
    QPair<int, int> rMeterData;
private:
    bool selected;
    int index;


    std::vector<class Region *> *regionList;
    std::vector<class Region *> *selectedRegionList;

    std::vector<class AudioEffect *> audioEffectChain;

    QString uuid;

    AudioManager *audioMan;

    Timeline *timeline;

    TrackControlsWidget *trackControlWidget;
    MixerChannelWidget *mixerChannelWidget;
    TrackGraphicItem *trackGraphicItem;

    QColor color;



    bool mute = false;
    bool solo;

    float gain = 0.0f;
    float gainNonLog = 0.0f;
    float pan = 0.0f;


};

#endif // TRACK_H
