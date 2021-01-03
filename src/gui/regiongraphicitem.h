#ifndef REGIONGRAPHICITEM_H
#define REGIONGRAPHICITEM_H

#include <QGraphicsItem>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QPainterPath>

#include <stdint.h>

class Timeline;
class Region;

#include "timeline.h"
#include "app/audiomanager.h"


class RegionGraphicItem : public QGraphicsItem
{
public:
    RegionGraphicItem(QGraphicsScene *_scene, QColor _color, Timeline *_timeline, Region *_region);
    float getGridLocation();
    void setGridLocation(float _value);
    void setGridLength(float _value);

    void setGhost(bool _isGhost);

    void setWaveform(std::shared_ptr<AudioBus> bus);
    void setHScaleFactor(int value);

    void setSelected(bool _selected);

    // QGraphicsItem interface
protected:
    //    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    // QGraphicsItem interface
protected:

    QColor outlineColor;
    QColor selectedColor;
    QColor regionColor;
    QColor waveFormColor;

    bool selected;

    int penWidth;
    int rounded;
    bool hasShadow;
    float thresholdShadow;
    QBrush mainBrush;
    QBrush waveformBrush;
    QPen mainPen;
    QPen waveformPen;

    QPixmap waveFormPixmap;
    int height;
    float gridLength;
    bool pressed = false;
    QPointF oldPos, oldMousePos;
    int oldTrackIndex;
    float gridLocation;
    QGraphicsScene *scene;
    Region *region;

    bool ghost;
    std::shared_ptr<AudioBus> bus;

    size_t samplesLength;
    // QGraphicsItem interface

    Timeline *timeline;

    int oldHScaleFactor;

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;


    virtual void keyPressEvent(QKeyEvent *event) override;

    void updateLengths();


private:

    float getMaximumSampleValueInRange(uint64_t firstSample, uint64_t lastSample, int channel);
    float getMinimumSampleValueInRange(uint64_t firstSample, uint64_t lastSample, int channel);
    uint64_t getFirstSampleIndexForPixel(uint64_t x, uint64_t widgetWidth, uint64_t totalNumSamples);
    float getYValueForSampleValue(float sample);

    bool waveFormRendered;

    int hScaleFactor;

    float lastGlr;

    void showContextMenu(QPoint pos);

    QMenu *tooltip;

    void showToolTip(QPoint pos);

};

#endif // RegionGraphicItem_H
