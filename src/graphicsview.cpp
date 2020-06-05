#include "graphicsview.h"
#include <QDebug>
#include "timeline.h"

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent, Timeline *_timeline):QGraphicsView(scene, parent), totalScaleFactor(1) {
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    this->setInteractive(true);
    //setDragMode(ScrollHandDrag);
    timeline = _timeline;
    timeline->pinchToZoom = false;
    //totalScaleFactor = timeline->hZoomFactor;
}

