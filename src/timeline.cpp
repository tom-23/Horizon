#include "timeline.h"
#include <QScrollBar>
#include <QVBoxLayout>

Timeline::Timeline(QGraphicsView *_trackRegions, QGraphicsView *_trackRuler, QWidget *_trackControls)
{
    trackRegions = _trackRegions;
    trackRuler = _trackRuler;
    trackControls = _trackControls;
    trackCount = 0;
    regionCount = 0;

    barCount = 8;
    barLength = 4;

    QGraphicsScene *regionsScene = new QGraphicsScene;
    regionsView = new GraphicsView(regionsScene);
    timelineGraphic = new TimelineGraphicWidget(regionsView, nullptr, 8, 4);
    trackRegions->setScene(regionsScene);

    QGraphicsScene *rulerScene = new QGraphicsScene;
    rulerView = new GraphicsView(rulerScene);
    rulerGraphic = new RulerGraphicWidget(rulerView, nullptr, 8);
    trackRuler->setScene(rulerScene);


    trackList = new vector<class Track *>;
    trackList->clear();

    regionList = new vector<class Region *>;
    regionList->clear();

    this->setHZoomFactor(100);


}

Timeline::~Timeline() {
    trackList->clear();

    delete trackList;
    qDebug() << "Clearing tracklist...";

}

void Timeline::setColorTheme(ThemeManager *_themeManager) {
    rulerGraphic->setColorTheme(_themeManager->getColor("lines-primary"), _themeManager->getColor("font-color-secondary"));
    timelineGraphic->setColorTheme(_themeManager->getColor("lines-primary"), _themeManager->getColor("lines-secondary"));
    for (int i = 0; i < trackCount; i++) {
        trackList->at(i)->setColorTheme(_themeManager->getColor("lines-primary"));
    }
}

void Timeline::setHZoomFactor(int _hZoomFactor) {
    hZoomFactor = _hZoomFactor;
    timelineGraphic->setHScaleFactor(hZoomFactor);
    rulerGraphic->setHScaleFactor(hZoomFactor);
    for (int i = 0; i < regionCount; i++) {
        regionList->at(i)->getRegionGraphicItem()->setHScaleFactor(hZoomFactor);
    }
    for (int i = 0; i < trackCount; i++) {
        trackList->at(i)->setHScaleFactor(hZoomFactor);
    }

    updateViewports();



}

void Timeline::updateViewports() {
    timelineGraphic->scene->setSceneRect(0,0, (barCount * hZoomFactor) - 10, (trackCount * 60) + 10);
    rulerGraphic->scene->setSceneRect(0,0, (barCount * hZoomFactor), rulerGraphic->height());
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
    QVBoxLayout* trackControlsLayout = qobject_cast<QVBoxLayout*>(trackControls->layout());
    qDebug() << "Vec Size:" << trackList->size();
    qDebug() << "Index:" << _index;
    trackControlsLayout->insertWidget(_index, tcw);

    Track *newTrack = new Track(tcw, timelineGraphic, _index);
    trackList->insert(trackList->begin() + _index, newTrack);

    tcw->show();
    trackCount = trackList->size();
    updateViewports();

}

void Timeline::addRegion(int _trackIndex) {

    RegionGraphicItem *rgi = new RegionGraphicItem(timelineGraphic->scene, QColor("#42f59b"), this);
    rgi->setHScaleFactor(hZoomFactor);
    timelineGraphic->scene->addItem(rgi);

    Region *newRegion = new Region(_trackIndex, this, rgi);
    regionList->insert(regionList->begin(), newRegion);
    regionCount = regionList->size();
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
}
