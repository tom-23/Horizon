#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QObject>

class GraphicsScene : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = nullptr);

signals:

};

#endif // GRAPHICSSCENE_H
