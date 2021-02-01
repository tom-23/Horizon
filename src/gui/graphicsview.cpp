#include "graphicsview.h"
#include <QDebug>
//#include "timeline.h"

GraphicsView::GraphicsView(QGraphicsScene *_scene, QWidget *parent, Timeline *_timeline):QGraphicsView(_scene, parent) {
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    this->setInteractive(true);
    //setDragMode(ScrollHandDrag);
    timeline = _timeline;
    scene = _scene;
    scene->setParent(this);

    timeline->pinchToZoom = false;
    //totalScaleFactor = timeline->hZoomFactor;
}

bool GraphicsView::viewportEvent(QEvent *event) {
    switch (event->type()) {
        case QEvent::WindowActivate:
            timeline->updateHeights();
            break;
         case QEvent::Resize:
            timeline->updateHeights();
            break;
        default:
            break;

    }

    return QGraphicsView::viewportEvent(event);
}


void GraphicsView::dragEnterEvent(QDragEnterEvent *event) {

    qDebug() << "EVENT" << event->source();
    event->accept();

}
