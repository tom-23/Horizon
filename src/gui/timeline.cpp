#include "timeline.h"

#include <QScrollBar>
#include <QVBoxLayout>

Timeline::Timeline(QWidget *_parent,
                   QGraphicsView *_trackRuler,
                   QScrollArea *_trackControls,
                   QLayout *_mainLayout,
                   QScrollBar *hScrollBar,
                   QScrollBar *vScrollBar)
{
    debug::out(3, "Loading timeline graphics...");

    trackRuler = _trackRuler;
    trackControls = _trackControls;
    parent = _parent;
    horizonalScrollBar = hScrollBar;
    trackCount = 0;
    regionCount = 0;

    barCount = 32;
    barLength = 4;


    debug::out(3, "Init Timeline...");

    regionSnapping = true;
    centerPlayhead = false;


    trackRegionsScene = new GraphicsScene();
    trackRegions = new GraphicsView(trackRegionsScene, parent, this);
    trackRegions->setScene(trackRegionsScene);
    trackRegions->setAcceptDrops(true);

    //trackRegions->updateGeometry();
    trackRegions->setParent(parent);
    //trackRegions->setRenderHints();
    trackRegions->setObjectName("trackRegions");
    trackRegions->setAlignment((Qt::AlignHCenter | Qt::AlignTop));

    trackRegions->show();
    trackRegions->setInteractive(true);
    trackRegions->setMouseTracking(true);
    trackRegions->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);

    //trackRegions->setDragMode(QGraphicsView::);
    trackRegions->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    trackRuler->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    trackRegions->setCacheMode(QGraphicsView::CacheBackground);
    trackRegions->setOptimizationFlags(QGraphicsView::DontClipPainter);
    trackRegions->setTransformationAnchor(QGraphicsView::AnchorViewCenter);


    _mainLayout->addWidget(trackRegions);

    timelineGraphic = new TimelineGraphicWidget(trackRegions, nullptr, barCount, barLength);
    //timelineGraphic->view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    parent->update();
    //parent->repaint();



    debug::out(3, "Init Ruler...");

    QGraphicsScene *rulerScene = new QGraphicsScene;
    trackRuler->setScene(rulerScene);
    rulerGraphic = new RulerGraphicWidget(trackRuler, nullptr, 8);
    rulerGraphic->setBarAmount(barCount);

    trackRegions->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    trackRegions->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    trackRegions->setHorizontalScrollBar(hScrollBar);
    trackRegions->setVerticalScrollBar(vScrollBar);

    QWidget *cornerWidget = new QWidget(trackRegions);
    cornerWidget->setObjectName("cornerWidget");
    trackRegions->setCornerWidget(cornerWidget);

    // Connect the track control's scroll action to the trackRegions vise-versa.
    QObject::connect(trackRegions->verticalScrollBar(), SIGNAL(valueChanged(int)), trackControls->verticalScrollBar(), SLOT(setValue(int)));
    QObject::connect(trackControls->verticalScrollBar(), SIGNAL(valueChanged(int)), trackRegions->verticalScrollBar(), SLOT(setValue(int)));


    // Connect the track ruler's scroll action to the trackRegions vise-versa.
    QObject::connect(trackRegions->horizontalScrollBar(), SIGNAL(valueChanged(int)), trackRuler->horizontalScrollBar(), SLOT(setValue(int)));
    QObject::connect(trackRuler->horizontalScrollBar(), SIGNAL(valueChanged(int)), trackRegions->horizontalScrollBar(), SLOT(setValue(int)));

    //QObject::connect(ui->overview, SIGNAL(valueChanged(int)), ui->trackRegions->horizontalScrollBar(), SLOT(setValue(int)));
    //connect(ui->trackRegions->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), ui->overview, SLOT(setRange(int, int)));
    //connect(ui->trackRegions->horizontalScrollBar(), SIGNAL(valueChanged(int)), ui->overview, SLOT(setValue(int)));


    debug::out(3, "Init Playhead...");

    playheadGraphic = new Playhead(trackRegions);
    trackRegionsScene->addItem(playheadGraphic);
    playheadGraphic->setZValue(101);

    debug::out(3, "Init Lists...");

    this->setHZoomFactor(100);

    regionGraphicList = {};
    trackGraphicList = {};

    debug::out(3, "Timeline Loaded!");

}

Timeline::~Timeline() {

}

void Timeline::setColorTheme(ThemeManager *_themeManager) {
    themeManager = _themeManager;
    rulerGraphic->setColorTheme(_themeManager->getColor("lines-primary"), _themeManager->getColor("font-color-secondary"));
    timelineGraphic->setColorTheme(_themeManager->getColor("lines-primary"), _themeManager->getColor("lines-secondary"));
    primaryColor = _themeManager->getColor("lines-primary");

    timelineGraphic->repaint();
    timelineGraphic->update();
    timelineGraphic->view->update();
    timelineGraphic->view->repaint();

    trackRegions->viewport()->repaint();
    trackRegions->viewport()->update();
}


QColor Timeline::getPrimaryColor() {
    return primaryColor;
}


void Timeline::clearAll() {
    regionGraphicList.clear();
    trackGraphicList.clear();
}

void Timeline::setHZoomFactor(int _hZoomFactor, QSlider *zoomSlider) {

    if (hZoomFactor > _hZoomFactor) {

    } else {

    }

    hZoomFactor = _hZoomFactor;

    trackRegions->sceneRect().center();

    timelineGraphic->setHScaleFactor(hZoomFactor);

    rulerGraphic->setHScaleFactor(hZoomFactor);

    playheadGraphic->setHScaleFactor(hZoomFactor);

    for (int i = 0; i < trackGraphicList.size(); i++) {
        trackGraphicList[i]->updateLength();
    }

    for (int i = 0; i < regionGraphicList.size(); i++) {
        regionGraphicList[i]->setHScaleFactor(hZoomFactor);
    }



    updateHeights();
}

void Timeline::updateHeights() {
    int height;
    if (trackRegions->height() > trackRegions->sceneRect().height()) {
        height = trackRegions->height();
    } else {
        height = trackRegions->sceneRect().height();
    }
    playheadGraphic->setHeight(height);
    timelineGraphic->setHeights(height);
    updateViewports();

}

void Timeline::updateViewports() {
    trackRegionsScene->setSceneRect(0,0, (barCount * hZoomFactor), (trackCount * 60) + 88);
    rulerGraphic->scene->setSceneRect(0,0, (barCount * hZoomFactor) + 10, rulerGraphic->height());
    for (int i = 0; i < trackGraphicList.size(); i++) {
        trackGraphicList[i]->updateLength();
    }
    trackRegions->viewport()->adjustSize();
    trackRegions->viewport()->update();
}

void Timeline::setBarAmount(int _barAmount) {
    qDebug() << "OLD BAR COUNT" << barCount;
    qDebug() << "NEW BAR COUNT" << _barAmount;
    barCount = _barAmount;
    timelineGraphic->setBarAmount(barCount);
    rulerGraphic->setBarAmount(barCount);

    // When adding new lines to the scene, regions will be put behind the items so we need to bring them to the foreground.
    this->setZRegionValues(barCount * barLength);

    setColorTheme(themeManager);

    updateViewports();
}

void Timeline::setBarLength(int _barLength) {
    barLength = _barLength;
    timelineGraphic->setBarLength(barLength);
    this->setZRegionValues(barCount * barLength);
}

int Timeline::getTrackCount() {
    return trackCount;
}


int Timeline::getRegionCount() {
    return regionCount;
}

void Timeline::addTrack(Track *_track) {

    TrackControlsWidget* tcw = new TrackControlsWidget(nullptr, _track);
    _track->setTrackControlsWidget(tcw);

    QVBoxLayout* trackControlsLayout = qobject_cast<QVBoxLayout*>(trackControls->widget()->layout());
    trackControlsLayout->insertWidget(trackControlsLayout->count() - 1, tcw);

    TrackGraphicItem *tgi = new TrackGraphicItem(timelineGraphic->scene, this, _track);

    timelineGraphic->scene->addItem(tgi);
    trackGraphicList.append(tgi);
    _track->setTrackGraphicsItem(tgi);
    tcw->show();

    trackCount += 1;

    updateViewports();
    updateHeights();


}

void Timeline::addRegion(Region *_region) {
    RegionGraphicItem *rgi = new RegionGraphicItem(trackRegionsScene, _region->getTrack()->getColor(), this, _region);
    _region->setRegionGraphicItem(rgi);
    trackRegionsScene->addItem(rgi);
    regionGraphicList.append(rgi);
    updateViewports();

    this->setZRegionValues(barCount * barLength);

    rgi->setHScaleFactor(hZoomFactor);

}

void Timeline::removeTrackGraphicsItem(TrackGraphicItem *tgi) {
    trackRegionsScene->removeItem(tgi);
}

void Timeline::removeTrack(Track *_track) {

}

void Timeline::removeRegion(Region *_region) {

}

void Timeline::setRegionTrack(int _oldTrackIndex, int _newTrackIndex) {
    qDebug() << "Old Val:" << _oldTrackIndex;
    qDebug() << "New Val:" << _newTrackIndex;
}


void Timeline::setZRegionValues(int _zValue) {
    qDebug() << "COUNT" << trackRegionsScene->items().count();
    //for (int i = 0; i < regionCount; i++) {
    //    regionList->at(i)->getRegionGraphicItem()->setZValue(i + _zValue + 1);
    //}
    playheadGraphic->setZValue((regionCount + _zValue) + 5);
}

void Timeline::setPlayheadLocation(float _location) {
    if (centerPlayhead) {

        int scrollValue = round((_location * float(hZoomFactor)) - (float(trackRegions->size().width()) / 2));
        horizonalScrollBar->setValue(scrollValue);


    }
    playheadGraphic->setGridLocation(_location);
}

void Timeline::stopCenterPlayhead() {
    centerPlayhead = false;
}

QGraphicsScene* Timeline::getScene() {
    return trackRegionsScene;
}

