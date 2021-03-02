#include "regiongraphicitem.h"

RegionGraphicItem::RegionGraphicItem(QGraphicsScene *_scene, QColor _color, Timeline *_timeline, Region *_region) : QGraphicsItem()
{
    setFlags(ItemIsMovable);
    regionColor = _color;
    scene = _scene;
    region = _region;
    timeline = _timeline;

    waveFormColor = regionColor.darker(80);

    outlineColor = _timeline->getPrimaryColor();
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

    ghost = true;
    this->prepareGeometryChange();
    gridLocation = region->getGridLocation();
    setY((region->getTrack()->getIndex() * 60) + 1);

    waveFormRendered = false;
    selected = false;


}

void RegionGraphicItem::setGridLength(float _value) {
    gridLength = _value;
    if ((gridLocation + gridLength) > timeline->barCount) { // if the length is bigger then the project size,
        // increase the project's bar count to accommodate.
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

    // FIXME: the last wave form block doesn't get cut off. Need to fix this.
    // this was a bugger to code...

    // this works by calculating a pixmap width:
    //
    // |--------------------------------------------------|
    // |                         1                        |
    // |--------------------------------------------------|
    //
    // and then dividing it up into blocks. we need to do this as Qt can't have large pixmap objects.
    // plus, it allows as to have a high resolution waveform.
    //
    // |------------|------------|------------|-----------|
    // |     1      |      2     |      3     |     4     |
    // |------------|------------|------------|-----------|
    //
    // we then stitch them back up on the ui and scale them horizontally relative to the hScaleFactor

    bus = _bus;
    samplesLength = bus->length(); // get length of all samples

    waveFormRendered = false; // don't paint the waveform if we haven't generated it.

    uint64_t imgWidth = gridLength * waveFormMaxReso; // the length of the pixmap according to the resolution.
    int blockAmount = (imgWidth / blockSize) + 1; // we then divide

    waveFormPixmapList = {}; // list of all the pixmaps

    for (int b = 0; b < blockAmount; b++) {

        debug::out(3, "Generating Block " + QString::number(b + 1).toStdString() + " of " + QString::number(blockAmount + 1).toStdString());

        // this is a smaller type of block. (blocks inside of block).
        uint64_t firstSample = b * blockSize; // we take the first sample of it
        uint64_t lastSample = ((b + 1) * blockSize); // then the last

        QPixmap pixmap(QSize(blockSize, this->boundingRect().height())); // create our block pixmap
        pixmap.fill(Qt::transparent);

        QPainter wavepainter(&pixmap);
        wavepainter.setPen(QPen(waveFormColor, 1)); // semi-transparent white

        for (uint64_t i = firstSample; i < lastSample; i++) {

            // i'll cover this in my main docment.
            const uint64_t firstSampleIndexForPixel = getFirstSampleIndexForPixel(i, imgWidth, samplesLength);
            const uint64_t lastSampleIndexForPixel = getFirstSampleIndexForPixel(i + 1, imgWidth, samplesLength)-1;

            const float largestSampleValueForPixel = getMaximumSampleValueInRange(firstSampleIndexForPixel, lastSampleIndexForPixel, 0);
            const float smallestSampleValueForPixel = getMinimumSampleValueInRange(firstSampleIndexForPixel, lastSampleIndexForPixel, 0);

            wavepainter.drawLine(QLineF(i - firstSample, getYValueForSampleValue(largestSampleValueForPixel), i - firstSample, getYValueForSampleValue(smallestSampleValueForPixel)));

        }
        wavepainter.end();
        waveFormPixmapList.append(pixmap);
        this->update();
    }
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

    for (uint64_t i = firstSample; i < lastSample; i++) {
        float sample = bus->channel(channel)->mutableData()[i];
        if (sample > maxSample) {

            maxSample = sample;
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

    QRectF rect = boundingRect();

    painter->setBrush(mainBrush);
    painter->setPen(mainPen);
    painter->drawRoundedRect(rect, 5, 5);

    painter->setPen(waveformPen);
    painter->setBrush(waveformBrush);

    if (waveFormRendered == true) {
        for (int i = 0; i < waveFormPixmapList.size(); i++) {
            int blockX = (i * (blockSize / waveFormMaxReso))  * hScaleFactor;
            int blockWidth = (blockSize / waveFormMaxReso) * hScaleFactor;
            QRect waveFormRect = QRect(blockX, rect.y(), blockWidth, rect.height());
            painter->drawPixmap(waveFormRect, waveFormPixmapList.at(i));

        }
    //    painter->drawPixmap(rect.toRect(), waveFormPixmap);
    }

    painter->setPen(mainPen);

    if (selected) {
        painter->setBrush(QBrush(mainPen.color()));
        painter->drawRoundedRect(QRect(rect.x(), rect.y(), rect.width(), 20), 5, 5);
        painter->drawRect(QRect(rect.x(), rect.y() + 5, rect.width(), 15));
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
    region->getTrack()->setRegionSelected(region, true);

    if (event->button() == Qt::RightButton) {

        showContextMenu(event->pos().toPoint());
    } else {
        pressed = true;
        oldGridLocation = gridLocation;
        QCursor cursor(Qt::BlankCursor);
        QApplication::setOverrideCursor(cursor);
        QApplication::changeOverrideCursor(cursor);
        oldMousePos = event->scenePos();
        oldPos = scenePos();
        int heightDiff = height + 4;
        oldTrackIndex = scenePos().y() / heightDiff;
    }


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
    setX((gridLocation - 1) * hScaleFactor);
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

    // if the track as changed track, change the track.
    if (newTrackIndex != oldTrackIndex) {

        Track *newTrack = region->getTrack()->getAudioManager()->getTrackByIndex(newTrackIndex);
        region->getTrack()->removeRegion(region);

        newTrack->setRegion(region);
        region->setTrack(newTrack);

        region->getTrack()->getAudioManager()->session->setRegionTrack(QString::fromStdString(region->getUUID()), QString::fromStdString(newTrack->getUUID()));

    }

    if (gridLocation != oldGridLocation) { // if the region has moved location, move location

        region->setGridLocation(gridLocation);
        region->getTrack()->getAudioManager()->session->moveRegion(QString::fromStdString(region->getUUID()), gridLocation);
        if (region->getTrack()->getAudioManager()->isPlaying == true) {
            region->schedule();
        }
    }
}
void RegionGraphicItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // not implemented
    showContextMenu(event->pos().toPoint());
}


void RegionGraphicItem::keyPressEvent(QKeyEvent *event) {
    // TODO: remove me
    qDebug() << "Key press captured";
}

void RegionGraphicItem::setGhost(bool _isGhost) {
    // ghost = file not loaded.
    ghost = _isGhost;
    if (ghost == true) {
        mainBrush = QBrush(Qt::transparent);
    } else {
        mainBrush = QBrush(regionColor);
    }
}

void RegionGraphicItem::setHScaleFactor(int value) {
    // when the user zooms in, we calculate where the positions of things should be
    prepareGeometryChange();
    hScaleFactor = value;
    if (pressed == false ) {
        setX((gridLocation - 1) * hScaleFactor);
    }
    update();
}

void RegionGraphicItem::setSelected(bool _selected) {
    selected = _selected;
    update();
}


void RegionGraphicItem::showContextMenu(QPoint pos) {

    //TODO: implement
    QGraphicsView *v = scene->views().first();
    QPointF sceneP = mapToScene(pos);
    QPoint viewP = v->mapFromScene(sceneP);
    QMenu menu(scene->views().first());
    menu.addAction("Test");
    menu.exec(v->viewport()->mapToGlobal(viewP));
}


void RegionGraphicItem::showToolTip(QPoint pos) {
    //TODO: implement
    QGraphicsView *v = scene->views().first();
    QPointF sceneP = mapToScene(pos);
    QPointF shiftedP = QPointF(sceneP.x() + 10, sceneP.y() - 10);
    QPoint viewP = v->mapFromScene(shiftedP);
}
