#ifndef GHOSTPLAYHEAD_H
#define GHOSTPLAYHEAD_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>

class Timeline;

class GhostPlayhead : public QGraphicsItem
{

public:
    GhostPlayhead(QGraphicsView *_view);
    void setHeight(int _height);

    void showGhostPlayhead();
    void hideGhostPlayhead();
    void setLocation(int x);
    bool isVisible;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QRectF boundingRect() const override;

private:
    QGraphicsView *view;
    QLine line;
    QPen pen;
    float gridLocation;

};

#endif // GHOSTPLAYHEAD_H
