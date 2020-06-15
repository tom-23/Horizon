#include "timelinegraphicwidget.h"

TimelineGraphicWidget::TimelineGraphicWidget(QGraphicsView *_view, QWidget *_parent, int _barAmount, int _barLength)
{

    view = _view;

    scene = view->scene();
    view->setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);
    barLines = new QList<QList<QGraphicsLineItem *>*>;

    hScaleFactor = 100;
    barAmount = _barAmount;
    barLength = _barLength;

    setAcceptDrops(true);


   // viewportPadding = new QGraphicsRectItem();
    //viewportPadding->setRect(0,0, (barAmount * hScaleFactor)+10, 5);
    //viewportPadding->setPen(Qt::NoPen);
    //scene->addItem(viewportPadding);

    QFont font = scene->font();
    font.setPointSize(10);

    // TimelineGraphicWidget Numbers and Lines
    for (int i = 0; i < barAmount; i++)
    {
        QList<QGraphicsLineItem *> *bar = new QList<QGraphicsLineItem *>;
        for (int beat = 0; beat < barLength; beat++) {

            if (beat == 0) {
                QGraphicsLineItem *line = scene->addLine(QLine(0, 0, 0, 400), QColor("#0f0f0f"));

                line->setPos(i * hScaleFactor, 0);

                bar->insert(bar->end(), line);

            } else {

                int hLocation = (hScaleFactor / barLength) * beat;
                QGraphicsLineItem *line = scene->addLine(QLine(0, 0, 0, 400), QColor("#1a1a1a"));
                line->setPos((i * hScaleFactor) + hLocation, 0);

                bar->insert(bar->end(), line);

            }

        }

        barLines->insert(barLines->end(), bar);


    }


    this->setMouseTracking(true);
    //connect(this, &TimelineGraphicWidget::mousePressEvent, this, &TimelineGraphicWidget::mousePressEventSlot);
    //connect(view, SIGNAL(dragEnterEvent(QDragEnterEvent)), this, SLOT(dragEnterEvent(QDragEnterEvent)));
}



void TimelineGraphicWidget::setHScaleFactor(int _value) {
    hScaleFactor = _value;
    for (int i = 0; i < barLines->size(); i++) {

        for (int beat = 0; beat < barLines->at(i)->size(); beat++) {
            if (beat == 0) {
                barLines->at(i)->at(beat)->setX(i * hScaleFactor);
            } else {
                int hLocation = (hScaleFactor / barLength) * beat;
                barLines->at(i)->at(beat)->setX((i * hScaleFactor) + hLocation);
            }

        }

    }


}

void TimelineGraphicWidget::setHeights(int _value) {
    for (int i = 0; i < barLines->size(); i++) {
        for (int beat = 0; beat < barLines->at(i)->size(); beat++) {
            barLines->at(i)->at(beat)->setLine(QLine(0, 0, 0, _value));
        }

    }
}

void TimelineGraphicWidget::setBarAmount(int _value) {
    if (barAmount < _value) {

        for (int i = barAmount; i < _value; i++)
        {
            QList<QGraphicsLineItem *> *bar = new QList<QGraphicsLineItem *>;
            for (int beat = 0; beat < barLength; beat++) {
                if (beat == 0) {
                    QGraphicsLineItem *line = scene->addLine(QLine(0, 0, 0, 400), QColor("#0f0f0f"));
                    line->setPos(i * hScaleFactor, 0);
                    bar->insert(bar->end(), line);

                } else {
                    int hLocation = (hScaleFactor / barLength) * beat;
                    QGraphicsLineItem *line = scene->addLine(QLine(0, 0, 0, 400), QColor("#1a1a1a"));
                    line->setPos((i * hScaleFactor) + hLocation, 0);

                    bar->insert(bar->end(), line);


                }

            }
            barLines->insert(barLines->end(), bar);
       }
       barAmount = _value;
    } else {
        barAmount = _value;
        for (int i = barAmount; i < barLines->size(); i++) {
            for (int beat = 0; beat < barLines->at(i)->size(); beat++) {
                scene->removeItem(barLines->at(i)->at(beat));

            }
            barLines->removeAt(i);

        }
    }


}

void TimelineGraphicWidget::setColorTheme(QColor primaryLinesColor, QColor secondaryLinesColor) {
    primaryColor = primaryLinesColor;
    seconardColor = secondaryLinesColor;
    for (int i = 0; i < barLines->size(); i++) {
        for (int beat = 0; beat < barLines->at(i)->size(); beat++) {
            if (beat == 0) {
                barLines->at(i)->at(beat)->setPen(primaryLinesColor);

            } else {
                barLines->at(i)->at(beat)->setPen(secondaryLinesColor);
            }
        }
    }
    this->repaint();
    this->update();
}



void TimelineGraphicWidget::setBarLength(int _value) {

    for (int i = 0; i < barLines->size(); i++) {
        for (int beat = 0; beat < barLines->at(i)->size(); beat++) {
            scene->removeItem(barLines->at(i)->at(beat));

        }

    }
    barLength = _value;
    barLines->clear();

    for (int i = 0; i < barAmount; i++)
    {
        QList<QGraphicsLineItem *> *bar = new QList<QGraphicsLineItem *>;
        for (int beat = 0; beat < barLength; beat++) {

            if (beat == 0) {
                QGraphicsLineItem *line = scene->addLine(QLine(0, 0, 0, 400), primaryColor);
                line->setPos(i * hScaleFactor, 0);

                bar->insert(bar->end(), line);

            } else {
                int hLocation = (hScaleFactor / barLength) * beat;
                QGraphicsLineItem *line = scene->addLine(QLine(0, 0, 0, 400), seconardColor);
                line->setPos((i * hScaleFactor) + hLocation, 0);

                bar->insert(bar->end(), line);

            }

        }
        barLines->insert(barLines->end(), bar);

    }

}

int TimelineGraphicWidget::getBarAmount() {
    return barAmount;
}

void TimelineGraphicWidget::resizeEventSlot(QResizeEvent *event) {


}

TimelineGraphicWidget::~TimelineGraphicWidget()
{

}

void TimelineGraphicWidget::dragEnterEventSlot(QDragEnterEvent *event) {
    event->acceptProposedAction();
    qDebug() << "Drag enter!";





}
