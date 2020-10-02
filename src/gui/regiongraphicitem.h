#ifndef REGIONGRAPHICITEM_H
#define REGIONGRAPHICITEM_H

#include <QGraphicsItem>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QPainterPath>



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

    void setWaveform(std::vector<const float *> _waveForm, unsigned long long int _length);




    // QGraphicsItem interface
protected:
    //    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    // QGraphicsItem interface
protected:

    QColor outlineColor;
    QColor selectedColor;
    QColor regionColor;
    QColor waveFormColor;

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
    float length;
    bool pressed = false;
    QPointF oldPos, oldMousePos;
    int oldTrackIndex;
    float gridLocation;
    QGraphicsScene *scene;
    Region *region;

    bool ghost;
    std::vector<const float *> waveForm;

    unsigned long long int samplesLength;
    // QGraphicsItem interface

    Timeline *timeline;

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void keyPressEvent(QKeyEvent *event) override;

private:

    float getMaximumSampleValueInRange(unsigned long long int firstSample, unsigned long long int lastSample, int channel);
    float getMinimumSampleValueInRange(unsigned long long int firstSample, unsigned long long int lastSample, int channel);
    unsigned long long int getFirstSampleIndexForPixel(unsigned long int x, unsigned long int widgetWidth, unsigned long long int totalNumSamples);
    float getYValueForSampleValue(float sample);

    bool waveFormRendered;

};

#endif // RegionGraphicItem_H
