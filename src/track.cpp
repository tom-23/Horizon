#include "track.h"
#include <iostream>

Track::Track(TrackControlsWidget *controlsWidget, TimelineGraphicWidget *_timeLineWidget, int _index) {
    index = _index;
    selected = false;
    QColor color = QColor("#0f0f0f");
    timeLineWidget = _timeLineWidget;
    seperator = timeLineWidget->scene->addLine(QLine(0, 0, timeLineWidget->scene->width(), 0), color);
    seperator->setY(((index + 1) * 60) - 1);

}

void Track::setColorTheme(QColor primaryColor) {
    seperator->setPen(primaryColor);
}


void Track::setHScaleFactor(int _hScaleFactor) {
    seperator->setLine(QLine(0, 0, (timeLineWidget->getBarAmount() * _hScaleFactor), 0));
}
