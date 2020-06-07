#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <iostream>
#include <QDebug>
#include <QTouchEvent>
class Timeline;

class GraphicsView : public QGraphicsView
{

    Q_OBJECT

public:
    GraphicsView(QGraphicsScene *_scene = 0, QWidget *parent = 0, Timeline *_timeline = 0);



    //virtual void mouseReleaseEvent(QMouseEvent *event) override;
    //virtual void mouseMoveEvent(QMouseEvent *event) override;
    //virtual void dragEnterEvent(QDragEnterEvent *event) override;
    //virtual void keyPressEvent(QKeyEvent *event) override;

    //void sendMousePressEventSignal(QMouseEvent *event);
    //void sendMouseMoveEventSignal(QMouseEvent *event);
    //void sendMouseReleaseEventSignal(QMouseEvent *event);
    //void sendDragEnterEventSignal(QDragEnterEvent *event);
    // QWidget interface

protected:

    virtual bool viewportEvent(QEvent *event) override;

private:
    qreal totalScaleFactor;
    Timeline *timeline;
    QGraphicsScene *scene;
};
#endif //GraphicsView.h
