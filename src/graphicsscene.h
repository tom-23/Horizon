#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = nullptr);

signals:

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
};

#endif // GRAPHICSSCENE_H
