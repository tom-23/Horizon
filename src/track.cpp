#include "track.h"
#include <iostream>

Track::Track(TrackControlsWidget *controlsWidget, TimelineGraphicWidget *timeLineWidget, int _index) {
    index = _index;
    selected = false;
    QColor color = QColor("#0f0f0f");
    //
    seperator = timeLineWidget->scene->addLine(QLine(0, 0, 400, 0), color);
    seperator->setY(((index + 1) * 60) - 1);

}
