#include "regiongraphicitem.h"
#include "timeline.h"

RegionGraphicItem::RegionGraphicItem(int _length, QColor _color, QGraphicsScene *_scene, Timeline *_timeline)
{
    setFlags(ItemIsMovable);
    color = _color;
    outlineColor = color.lighter(30);
    selectedColor = QColor(200,30,180);
    selectedColorOutline = selectedColor.lighter(30);
    //        rounded = 3;
    //        hasShadow = false;
    //        thresholdShadow=0.0f;
    brush = QBrush(color);
    pen = QPen(outlineColor, penWidth);
    pen.setCapStyle(Qt::RoundCap);
    length = _length;
    hScaleFactor = 100;
    height = 60;
    oldPos = scenePos();
    scene = _scene;
}


RegionGraphicItem::RegionGraphicItem(QGraphicsScene *_scene, QColor _color, Timeline *_timeline) : QGraphicsItem()
{
    setFlags(ItemIsMovable);
    color = _color;
    outlineColor = QColor("#0f0f0f");
    selectedColor = selectedColor.lighter(30);
    selectedColorOutline = QColor("#0f0f0f");
    //        rounded = 3;
    //        hasShadow = false;
    //        thresholdShadow=0.0f;
    brush = QBrush(color);
    pen = QPen(outlineColor, 1);
    pen.setCapStyle(Qt::RoundCap);
    hScaleFactor = 100;
    length = 1;
    height = 56;
    oldPos = pos();
    scene = _scene;
    timeline = _timeline;
    gridLocation = 1;
}


QRectF RegionGraphicItem::boundingRect() const
{
    return QRectF(0, 1, length * hScaleFactor, height);
}

void RegionGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->setBrush(brush);
    painter->setPen(pen);
    painter->drawRoundedRect(boundingRect(), 5, 5);
    painter->setBrush(outlineColor);
    QFont font = scene->font();
    font.setPointSize(10);
    QFontMetricsF fontMetrics(font);
    QString text("Audio Region #1");
    int heightFont = fontMetrics.boundingRect(text).height();
    painter->drawText(5, heightFont + 3, text);
}



//QVariant RegionGraphicItem::itemChange(GraphicsItemChange change, const QVariant &value) {
//        if (change == ItemPositionChange && scene()) {
//            // value is the new position.
//            QPointF newPos = value.toPointF();

//            newPos.setY(y());
//            if(newPos.x() < 0){
//                newPos.setX(0);
//            }
//    //        newPos = qBound(QPointF(0,0),newPos,QPointF(10000,0));
//            return newPos;
//        }
//        return QGraphicsItem::itemChange(change, value);
//    }

void RegionGraphicItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Press";
    pressed = true;
    oldMousePos = event->scenePos();
    oldPos = scenePos();
    int heightDiff = height + 4;
    oldTrackIndex = scenePos().y() / heightDiff;
}
void RegionGraphicItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{


    if (pressed)
    {
        qDebug() << "Mouse Location:" << event->pos().x();
        // Code Below handles the Y movement of the region:

        QPointF newPos = event->scenePos();

        int yDiff = newPos.y() - oldPos.y();

        int heightDiff = height + 4;

        if (abs(yDiff) > heightDiff)
        {


            newPos.setY(oldPos.y() + (int)(yDiff / heightDiff) * heightDiff); //*((int)(yDiff/30))>0?1:0);
            setY(newPos.y());
            //qDebug() << "new";
            //                tempOldPos.setY(newPos.y() - oldPos.y());
        }
        else
        {
            setY(oldPos.y());
            //qDebug() << "old";
        }

        // Code below handles the X movement of the region:

        gridLocation = (scenePos().x() / hScaleFactor) + 1;

        int dx = (newPos - oldMousePos).x();
        setX(oldPos.x() + dx);
        //        setY(newPos.y());
    }
}

void RegionGraphicItem::setHScaleFactor(int _value) {
    hScaleFactor = _value;
    setX((gridLocation - 1) * hScaleFactor);

}

float RegionGraphicItem::getGridLocation() {
    return gridLocation;
}

void RegionGraphicItem::setGridLocation(float _value) {
    gridLocation = _value;

}

void RegionGraphicItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Release";
    pressed = false;
    oldMousePos = event->scenePos();
    oldPos = scenePos();
    int heightDiff = height + 4;
    int newTrackIndex = scenePos().y() / heightDiff;

    if (newTrackIndex != oldTrackIndex) {

        qDebug() << "Track Change!";
        timeline->setRegionTrack(oldTrackIndex, newTrackIndex);

    }
}
void RegionGraphicItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Double Click";
}


void RegionGraphicItem::keyPressEvent(QKeyEvent *event) {
    qDebug() << "Key press captured";
}
