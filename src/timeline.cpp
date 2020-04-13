#include "timeline.h"
#include <QScrollBar>
#include <QVBoxLayout>

Timeline::Timeline(QGraphicsView *_trackRegions, QGraphicsView *_trackRuler, QWidget *_trackControls)
{
    trackRegions = _trackRegions;
    trackRuler = _trackRuler;
    trackControls = _trackControls;

    QGraphicsScene *regionsScene = new QGraphicsScene;
    regionsView = new GraphicsView(regionsScene);
    timelineGraphic = new TimelineGraphicWidget(regionsView);
    trackRegions->setScene(regionsScene);

    QGraphicsScene *rulerScene = new QGraphicsScene;
    rulerView = new GraphicsView(rulerScene);
    rulerGraphic = new RulerGraphicWidget(rulerView);
    trackRuler->setScene(rulerScene);

    trackList = new vector<class Track *>;

}

Timeline::~Timeline() {
    trackList->clear();
    trackList->~vector();
}

void Timeline::setHZoomFactor(int _hZoomFactor) {
    hZoomFactor = _hZoomFactor;
    timelineGraphic->setHScaleFactor(hZoomFactor);
    rulerGraphic->setHScaleFactor(hZoomFactor);

}

void Timeline::setBarAmount(int _barAmount) {
    barCount = _barAmount;
    timelineGraphic->setBarAmount(barCount);
    rulerGraphic->setBarAmount(barCount);
}

void Timeline::setBarLength(int _barLength) {
    barLength = _barLength;
    timelineGraphic->setBarLength(barLength);
}

void Timeline::addTrack(int _index) {
    trackCount = trackCount + 1;

    TrackControlsWidget* tcw = new TrackControlsWidget();
    QVBoxLayout* trackControlsLayout = qobject_cast<QVBoxLayout*>(trackControls->layout());

    trackControlsLayout->insertWidget(_index, tcw);
    Track *newTrack = new Track(tcw, timelineGraphic, _index);
    //trackList->insert(trackList->begin() + _index, newTrack);
    tcw->show();

}


void Timeline::removeTrack(int _index) {
    trackCount = trackCount - 1;

}
