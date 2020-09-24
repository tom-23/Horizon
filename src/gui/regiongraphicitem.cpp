#include "regiongraphicitem.h"

RegionGraphicItem::RegionGraphicItem(int _length, QColor _color, QGraphicsScene *_scene, Timeline *_timeline, Region *_region)
{
    setFlags(ItemIsMovable);
    regionColor = _color;
    color = Qt::transparent;
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
    height = 60;
    oldPos = scenePos();
    scene = _scene;
    region = _region;
    timeline = _timeline;
}


RegionGraphicItem::RegionGraphicItem(QGraphicsScene *_scene, QColor _color, Timeline *_timeline, Region *_region) : QGraphicsItem()
{
    setFlags(ItemIsMovable);
    regionColor = _color;
    color = Qt::transparent;
    outlineColor = QColor("#0f0f0f");
    selectedColor = selectedColor.lighter(30);
    selectedColorOutline = QColor("#0f0f0f");
    //        rounded = 3;
    //        hasShadow = false;
    //        thresholdShadow=0.0f;
    brush = QBrush(color);
    pen = QPen(outlineColor, 1);
    pen.setCapStyle(Qt::RoundCap);
    length = 1;
    height = 56;
    oldPos = pos();
    scene = _scene;
    region = _region;
    timeline = _timeline;
    ghost = true;
    gridLocation = 1;
    waveFormPoints = {};
    setY((region->getTrack()->getIndex() * 60) + 1);
}

void RegionGraphicItem::setGridLength(float _value) {
    length = _value;
    scene->update();
    timeline->updateViewports();
}

QRectF RegionGraphicItem::boundingRect() const
{
    return QRectF(0, 1, length * timeline->hZoomFactor, height);
}

void RegionGraphicItem::setWaveform(std::vector<std::vector<float>> waveForm) {

    qDebug() << "Setting Waveform...";

    int samples = this->length * 1000;
    qDebug() << samples;
    int blockSize = floor((int)waveForm.at(0).size() / samples);

    waveFormPoints.push_back({});

    for (int i = 0; i < samples; i++) {
        int blockStart = blockSize * i;
        float sum = 0.0;

        for (int j = 0; j < blockSize; j++) {
            sum = sum + waveForm.at(0).at(blockStart + j);
        }
        waveFormPoints.at(0).push_back(sum / blockSize);
    }


}

void RegionGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->setBrush(brush);
    painter->setPen(pen);
    painter->drawRoundedRect(boundingRect(), 5, 5);
    painter->setBrush(outlineColor);


    qDebug() << "Draw..." << waveFormPoints.size();

    if (waveFormPoints.size() != 0) {
        qDebug() << "Starting paint";

        QPainterPath waveformPath;

        waveformPath.moveTo(0, 0);

        QVector<QPointF> points;
        int midPoint = this->height / 2;

        for (int i = 0; i < (int)waveFormPoints.at(0).size(); i++) {

            float dataPointY = (waveFormPoints.at(0).at(i) * midPoint) + midPoint;
            qDebug() << dataPointY;
            points.push_back(QPointF(i / 10, dataPointY));
        }

        QPolygonF waveformPolygon(points);



        waveformPath.addPolygon(waveformPolygon);

        painter->drawPath(waveformPath);


    }




    QFont font = scene->font();
    font.setPixelSize(10);
    font.setBold(true);
    QFontMetricsF fontMetrics(font);
    QString text = QString::fromStdString(region->getRegionName());
    int heightFont = fontMetrics.boundingRect(text).height();
    painter->drawText(5, heightFont + 3, text);
    if (pressed == false ) {
        setX((gridLocation - 1) * timeline->hZoomFactor);
    }
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
    pressed = true;
    QCursor cursor(Qt::BlankCursor);
    QApplication::setOverrideCursor(cursor);
    QApplication::changeOverrideCursor(cursor);
    oldMousePos = event->scenePos();
    oldPos = scenePos();
    int heightDiff = height + 4;
    oldTrackIndex = scenePos().y() / heightDiff;
    timeline->suspendGhostPlayhead = true;
    timeline->ghostPlayheadGraphic->setLocation(this->x());
}
void RegionGraphicItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{


    if (pressed)
    {
        // qDebug() << "Mouse Location:" << event->pos().x();
        // Code Below handles the Y movement of the region:

        QPointF newPos = event->scenePos();

        int yDiff = newPos.y() - oldPos.y();

        int heightDiff = height + 4;

        int yValue;

        if (abs(yDiff) > heightDiff)
        {


            newPos.setY(oldPos.y() + (int)(yDiff / heightDiff) * heightDiff); //*((int)(yDiff/30))>0?1:0);
            yValue = newPos.y();
            //                tempOldPos.setY(newPos.y() - oldPos.y());
        }
        else
        {
            yValue = oldPos.y();
        }

        // Code below handles the X movement of the region:

        int newTrackIndex = yValue / heightDiff;

        if (newTrackIndex <= (region->getTrack()->getAudioManager()->getTrackListCount() - 1)) {
            setY(yValue);
        }



        int dx = (newPos - oldMousePos).x();

        if (timeline->regionSnapping == false) {
            setX(oldPos.x() + dx);

        } else {


            float division = 0.0f;
            if (timeline->hZoomFactor >= 150) {
                    division = timeline->barLength * 2;
            } else {
                    division = float(timeline->barLength);
            }
            float gridLoc = ((oldPos.x() + dx) / timeline->hZoomFactor);
            float glr = float(round((gridLoc + 1) * division)) / division;
            setX((glr - 1) * timeline->hZoomFactor);
        }

        gridLocation = (scenePos().x() / timeline->hZoomFactor) + 1;
        timeline->ghostPlayheadGraphic->setLocation(this->x());
    }
}

float RegionGraphicItem::getGridLocation() {
    return gridLocation;
}

void RegionGraphicItem::setGridLocation(float _value) {
    gridLocation = _value;

}

void RegionGraphicItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    QCursor cursor(Qt::ArrowCursor);
    QApplication::setOverrideCursor(cursor);
    QApplication::changeOverrideCursor(cursor);
    oldMousePos = event->scenePos();
    oldPos = scenePos();
    int heightDiff = height + 4;
    int newTrackIndex = scenePos().y() / heightDiff;

    if (newTrackIndex != oldTrackIndex) {

        Track *newTrack = region->getTrack()->getAudioManager()->getTrackByIndex(newTrackIndex);
        region->getTrack()->removeRegion(region);

        newTrack->setRegion(region);
        region->setTrack(newTrack);

    }

    region->setGridLocation(gridLocation);
    if (region->getTrack()->getAudioManager()->isPlaying == true) {
        region->schedule();
    }
    timeline->ghostPlayheadGraphic->setLocation(mapToScene(event->pos()).x());
    timeline->suspendGhostPlayhead = false;

}
void RegionGraphicItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Double Click";
}


void RegionGraphicItem::keyPressEvent(QKeyEvent *event) {
    qDebug() << "Key press captured";
}

void RegionGraphicItem::setGhost(bool _isGhost) {
    ghost = _isGhost;
    if (ghost == true) {
        color = Qt::transparent;
        brush = QBrush(color);
    } else {
        color = regionColor;
        brush = QBrush(color);
    }
}
