#include "graphicsscene.h"
#include <QDebug>


GraphicsScene::GraphicsScene(QObject *parent) : QGraphicsScene(parent)
{

}

void GraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
    event->acceptProposedAction();
}
