#include "graphicsview.h"
#include <QDebug>
//#include "timeline.h"

GraphicsView::GraphicsView(QGraphicsScene *_scene, QWidget *parent, Timeline *_timeline):QGraphicsView(_scene, parent), totalScaleFactor(1) {
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    this->setInteractive(true);
    //setDragMode(ScrollHandDrag);
    timeline = _timeline;
    scene = _scene;
    timeline->pinchToZoom = false;
    //totalScaleFactor = timeline->hZoomFactor;
}

bool GraphicsView::viewportEvent(QEvent *event) {
    switch (event->type()) {
        case QEvent::WindowActivate: {
            timeline->updateHeights();
        }
        case QEvent::Enter: {
            timeline->ghostPlayheadGraphic->showGhostPlayhead();
        }
        case QEvent::Leave: {
            timeline->ghostPlayheadGraphic->hideGhostPlayhead();
        }
        case QEvent::MouseMove: {
            if (timeline->suspendGhostPlayhead == false) {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                timeline->ghostPlayheadGraphic->setLocation(mapToScene(mouseEvent->pos()).x());
            }
        }
         case QEvent::Resize: {
            timeline->updateHeights();
        }
        default:

            break;
        }

    return QGraphicsView::viewportEvent(event);
}

