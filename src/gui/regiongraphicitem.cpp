#include "regiongraphicitem.h"

RegionGraphicItem::RegionGraphicItem(QGraphicsScene *_scene, QColor _color, Timeline *_timeline, Region *_region) : QGraphicsItem()
{
    setFlags(ItemIsMovable);
    regionColor = _color;
    waveFormColor = regionColor.darker(80);

    outlineColor = QColor("#0f0f0f");
    selectedColor = selectedColor.lighter(30);
    //        rounded = 3;
    //        hasShadow = false;
    //        thresholdShadow=0.0f;
    hScaleFactor = 100;
    mainBrush = QBrush(Qt::transparent);
    mainPen = QPen(outlineColor, 1);
    waveformBrush = QBrush(waveFormColor);
    waveformPen = QPen(waveFormColor.darker(70), 1);
    mainPen.setCapStyle(Qt::RoundCap);
    gridLength = 1;
    height = 56;
    oldPos = pos();
    scene = _scene;
    region = _region;
    timeline = _timeline;
    ghost = true;
    this->prepareGeometryChange();
    gridLocation = region->getGridLocation();
    setY((region->getTrack()->getIndex() * 60) + 1);

    waveFormRendered = false;


}

void RegionGraphicItem::setGridLength(float _value) {
    gridLength = _value;
    if ((gridLocation + gridLength) > timeline->barCount) {
        timeline->setBarAmount(ceil(gridLocation + gridLength) + 1);
    }
    scene->update();
    timeline->updateViewports();
    prepareGeometryChange();

}

QRectF RegionGraphicItem::boundingRect() const
{
    return QRectF(0, 1, float(gridLength * hScaleFactor), float(height));
}

void RegionGraphicItem::setWaveform(std::shared_ptr<AudioBus> _bus) {

    bus = _bus;
    samplesLength = bus->length();

    waveFormRendered = false;

    uint64_t imgWidth = gridLength * 300;

    QPixmap pixmap(QSize(imgWidth, this->boundingRect().height()));
    pixmap.fill(Qt::transparent);

    QPainter wavepainter(&pixmap);
    wavepainter.setPen(QPen(waveFormColor, 1));




    for (uint64_t i = 0; i < imgWidth; i++) {

        const uint64_t firstSampleIndexForPixel = getFirstSampleIndexForPixel(i, imgWidth, samplesLength);
        const uint64_t lastSampleIndexForPixel = getFirstSampleIndexForPixel(i + 1, imgWidth, samplesLength)-1;

        const float largestSampleValueForPixel = getMaximumSampleValueInRange(firstSampleIndexForPixel, lastSampleIndexForPixel, 0);
        const float smallestSampleValueForPixel = getMinimumSampleValueInRange(firstSampleIndexForPixel, lastSampleIndexForPixel, 0);

        wavepainter.drawLine(QLineF(i, getYValueForSampleValue(largestSampleValueForPixel), i, getYValueForSampleValue(smallestSampleValueForPixel)));
        qDebug() << "Y VAL MAX" << largestSampleValueForPixel;
        //qDebug() << "Y VAL MIN" << getYValueForSampleValue(smallestSampleValueForPixel);
    }

    qDebug() << "Image Width" << imgWidth;
    qDebug() << "LENGTH" << samplesLength;
    //qDebug() << "ARRAY SIZE" << waveForm.at(0);
    wavepainter.end();

    waveFormPixmap = pixmap;
    waveFormRendered = true;


}

uint64_t RegionGraphicItem::getFirstSampleIndexForPixel(uint64_t x, uint64_t widgetWidth, uint64_t totalNumSamples) {
   return (totalNumSamples * x)/widgetWidth;
}

float RegionGraphicItem::getMaximumSampleValueInRange(uint64_t firstSample, uint64_t lastSample, int channel) {
    float maxSample = 0.0f;

    if (firstSample >= samplesLength) {
        firstSample = samplesLength - 1;
    }
    if (lastSample >= samplesLength) {
        lastSample = samplesLength - 1;
    }
    qDebug() << "Sample value" << bus->channel(channel)->data()[firstSample];
    qDebug() << "Sample Index" << firstSample;
    for (uint64_t i = firstSample; i < lastSample; i++) {

        if (bus->channel(channel)->data()[i] > maxSample) {

            maxSample = bus->channel(channel)->data()[i];
        }
    }
    return maxSample;
}

float RegionGraphicItem::getMinimumSampleValueInRange(uint64_t firstSample, uint64_t lastSample, int channel) {
    float minSample = 0.0f;

    if (firstSample >= samplesLength) {
        firstSample = samplesLength - 1;
    }
    if (lastSample >= samplesLength) {
        lastSample = samplesLength - 1;
    }


    for (uint64_t i = firstSample; i < lastSample; i++) {
        if (bus->channel(channel)->data()[i] < minSample) {
            minSample = bus->channel(channel)->data()[i];
        }
    }
    return minSample;
}

float RegionGraphicItem::getYValueForSampleValue(float sample) {
    int midPoint = ((this->height - 20) / 2) + 2;
    return (sample * midPoint) + midPoint + 10;
}

void RegionGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    selected = false;

    painter->setBrush(mainBrush);
    painter->setPen(mainPen);
    painter->drawRoundedRect(boundingRect(), 5, 5);

    painter->setPen(waveformPen);
    painter->setBrush(waveformBrush);

    if (waveFormRendered == true) {
        painter->drawPixmap(boundingRect().toRect(), waveFormPixmap);
    }

    painter->setPen(mainPen);

    if (selected) {
        painter->setBrush(QBrush(mainPen.color()));
        painter->drawRoundedRect(QRect(boundingRect().x(), boundingRect().y(), boundingRect().width(), 20), 5, 5);
        painter->drawRect(QRect(boundingRect().x(), boundingRect().y() + 5, boundingRect().width(), 15));
    }


    QFont font = scene->font();
    font.setPixelSize(10);
    font.setBold(true);
    QFontMetricsF fontMetrics(font);
    QString text = QString::fromStdString(region->getRegionName());
    int heightFont = fontMetrics.boundingRect(text).height();
    if (selected) {
        painter->setPen(QPen(mainBrush.color(), 1));
    }
    painter->drawText(5, heightFont + 3, text);
    if (pressed == false ) {
        setX((gridLocation - 1) * hScaleFactor);
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
            if (hScaleFactor >= 150) {
                    division = timeline->barLength * 2;
            } else {
                    division = float(timeline->barLength);
            }
            float gridLoc = ((oldPos.x() + dx) / hScaleFactor);
            float glr = float(round((gridLoc + 1) * division)) / division;
            setX((glr - 1) * hScaleFactor);
            if (glr != lastGlr) {
                lastGlr = glr;
                #ifdef __APPLE__
                util::macTouchpadVibrateSmall();
                #endif
            }

        }

        gridLocation = (scenePos().x() / hScaleFactor) + 1;
    }
}

float RegionGraphicItem::getGridLocation() {
    return gridLocation;
}

void RegionGraphicItem::setGridLocation(float _value) {
    gridLocation = _value;
    if ((gridLocation + gridLength) > timeline->barCount) {
        timeline->setBarAmount(ceil(gridLocation + gridLength) + 1);
    }
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

        region->getTrack()->getAudioManager()->session->setRegionTrack(QString::fromStdString(region->getUUID()), QString::fromStdString(newTrack->getUUID()));

    }

    region->setGridLocation(gridLocation);
    region->getTrack()->getAudioManager()->session->moveRegion(QString::fromStdString(region->getUUID()), gridLocation);

    if (region->getTrack()->getAudioManager()->isPlaying == true) {
        region->schedule();
    }
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
        mainBrush = QBrush(Qt::transparent);
    } else {
        mainBrush = QBrush(regionColor);
    }
}

void RegionGraphicItem::setHScaleFactor(int value) {
    hScaleFactor = value;
    prepareGeometryChange();

}
