#include "graphicsscene.h"
#include <QDebug>


GraphicsScene::GraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    qDebug() << "Inheritance seems to be working";


}

void GraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
    event->acceptProposedAction();
}
