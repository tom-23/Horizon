#include "timeline.h"
#include "debug.h"
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
    trackCount = 0;
    regionCount = 0;

    barCount = 8;
    barLength = 4;


    debug::out(3, "Init Timeline...");

    QGraphicsScene *regionsScene = new GraphicsScene();

    trackRegions = new GraphicsView(regionsScene, parent, this);
    trackRegions->updateGeometry();
    trackRegions->setParent(parent);
    trackRegions->setObjectName("trackRegions");
    trackRegions->setAlignment((Qt::AlignLeft | Qt::AlignTop));
    _mainLayout->addWidget(trackRegions);
    trackRegions->show();
    trackRegions->setInteractive(true);
    trackRegions->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    trackRegions->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);




    timelineGraphic = new TimelineGraphicWidget(trackRegions, nullptr, 8, 4);
    parent->update();
    parent->repaint();



    debug::out(3, "Init Ruler...");

    QGraphicsScene *rulerScene = new QGraphicsScene;
    trackRuler->setScene(rulerScene);
    rulerGraphic = new RulerGraphicWidget(trackRuler, nullptr, 8);



    QObject::connect(vScrollBar, SIGNAL(valueChanged(int)), trackRegions->verticalScrollBar(), SLOT(setValue(int)));
    QObject::connect(hScrollBar, SIGNAL(valueChanged(int)), trackRegions->horizontalScrollBar(), SLOT(setValue(int)));

    QObject::connect(trackRegions->verticalScrollBar(), SIGNAL(valueChanged(int)), vScrollBar, SLOT(setValue(int)));
    QObject::connect(trackRegions->horizontalScrollBar(), SIGNAL(valueChanged(int)), hScrollBar, SLOT(setValue(int)));

    QObject::connect(trackRegions->verticalScrollBar(), SIGNAL(rangeChanged(int, int)), vScrollBar, SLOT(setRange(int, int)));
    QObject::connect(trackRegions->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), hScrollBar, SLOT(setRange(int, int)));

    QObject::connect(trackRegions->verticalScrollBar(), SIGNAL(valueChanged(int)), trackControls->verticalScrollBar(), SLOT(setValue(int)));
    QObject::connect(trackControls->verticalScrollBar(), SIGNAL(valueChanged(int)), trackRegions->verticalScrollBar(), SLOT(setValue(int)));


    QObject::connect(trackRegions->horizontalScrollBar(), SIGNAL(valueChanged(int)), trackRuler->horizontalScrollBar(), SLOT(setValue(int)));

    //QObject::connect(trackRuler->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(on_trackRuler_sliderChange(int)));

    //QObject::connect(ui->overview, SIGNAL(valueChanged(int)), ui->trackRegions->horizontalScrollBar(), SLOT(setValue(int)));
    //connect(ui->trackRegions->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), ui->overview, SLOT(setRange(int, int)));
    //connect(ui->trackRegions->horizontalScrollBar(), SIGNAL(valueChanged(int)), ui->overview, SLOT(setValue(int)));


    debug::out(3, "Init Playhead...");

    playheadGraphic = new Playhead(trackRegions);
    regionsScene->addItem(playheadGraphic);
    playheadGraphic->setZValue(101);

    debug::out(3, "Init GhostPlayhead...");

    ghostPlayheadGraphic = new GhostPlayhead(trackRegions);
    regionsScene->addItem(ghostPlayheadGraphic);
    playheadGraphic->setZValue(101);

    debug::out(3, "Init Lists...");

    trackList = new vector<class Track *>;
    trackList->clear();

    regionList = new vector<class Region *>;
    regionList->clear();

    this->setHZoomFactor(100);




    debug::out(3, "Timeline Loaded!");

}

Timeline::~Timeline() {
    trackList->clear();

    delete trackList;
    debug::out(3, "Clearing trackList...");

}

void Timeline::setColorTheme(ThemeManager *_themeManager) {
    rulerGraphic->setColorTheme(_themeManager->getColor("lines-primary"), _themeManager->getColor("font-color-secondary"));
    timelineGraphic->setColorTheme(_themeManager->getColor("lines-primary"), _themeManager->getColor("lines-secondary"));
    for (int i = 0; i < trackCount; i++) {
        trackList->at(i)->setColorTheme(_themeManager->getColor("lines-primary"));
    }
}

void Timeline::setHZoomFactor(int _hZoomFactor, QSlider *zoomSlider) {
    hZoomFactor = _hZoomFactor;

    timelineGraphic->setHScaleFactor(hZoomFactor);

    rulerGraphic->setHScaleFactor(hZoomFactor);

    playheadGraphic->setHScaleFactor(hZoomFactor);

    for (int i = 0; i < regionCount; i++) {
        regionList->at(i)->getRegionGraphicItem()->setHScaleFactor(hZoomFactor);
    }
    for (int i = 0; i < trackCount; i++) {
        trackList->at(i)->setHScaleFactor(hZoomFactor);
    }

    updateViewports();
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
    ghostPlayheadGraphic->setHeight(height);
    updateViewports();

}

void Timeline::updateViewports() {
    timelineGraphic->scene->setSceneRect(0,0, (barCount * hZoomFactor), (trackCount * 60) + 88);
    rulerGraphic->scene->setSceneRect(0,0, (barCount * hZoomFactor) + 10, rulerGraphic->height());

}

void Timeline::setBarAmount(int _barAmount) {
    barCount = _barAmount;
    timelineGraphic->setBarAmount(barCount);
    rulerGraphic->setBarAmount(barCount);
    // When adding new lines to the scene, regions will be put behind the items so we need to bring them to the foreground.
    this->setZRegionValues(barCount * barLength);
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

void Timeline::addTrack(int _index) {


    TrackControlsWidget* tcw = new TrackControlsWidget();
    QVBoxLayout* trackControlsLayout = qobject_cast<QVBoxLayout*>(trackControls->widget()->layout());
    qDebug() << "Vec Size:" << trackList->size();
    qDebug() << "Index:" << _index;
    trackControlsLayout->insertWidget(_index, tcw);

    Track *newTrack = new Track(tcw, timelineGraphic, _index);
    trackList->insert(trackList->begin() + _index, newTrack);

    tcw->show();
    trackCount = trackList->size();
    updateViewports();
    updateHeights();
}

void Timeline::addRegion(int _trackIndex) {

    RegionGraphicItem *rgi = new RegionGraphicItem(timelineGraphic->scene, QColor("#42f59b"), this);
    rgi->setHScaleFactor(hZoomFactor);
    timelineGraphic->scene->addItem(rgi);

    Region *newRegion = new Region(_trackIndex, this, rgi);
    regionList->insert(regionList->begin(), newRegion);
    regionCount = regionList->size();

    this->setZRegionValues(barCount * barLength);
}


void Timeline::removeTrack(int _index) {
    trackCount = trackCount - 1;

}

void Timeline::setRegionTrack(int _oldTrackIndex, int _newTrackIndex) {
    qDebug() << "Old Val:" << _oldTrackIndex;
    qDebug() << "New Val:" << _newTrackIndex;
}


void Timeline::setZRegionValues(int _zValue) {
    for (int i = 0; i < regionCount; i++) {
        regionList->at(i)->getRegionGraphicItem()->setZValue(i + _zValue + 1);
    }
    ghostPlayheadGraphic->setZValue((regionCount + _zValue) + 5);
    playheadGraphic->setZValue((regionCount + _zValue) + 5);
}

void Timeline::setPlayheadLocation(float _location) {
    playheadGraphic->setGridLocation(_location);
}

