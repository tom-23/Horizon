#include "rulergraphicwidget.h"

RulerGraphicWidget::RulerGraphicWidget(QGraphicsView *_view, QWidget *_parent, int _barAmount)
{
    if (_view)
        view = _view;
    else
        view = new QGraphicsView(this);
    scene = view->scene();
    setMouseTracking(true);
    barLines = new QList<QGraphicsLineItem *>;
    barNumbers = new QList<QGraphicsTextItem *>;

    hScaleFactor = 100;
    barAmount = _barAmount;
    QFont font = scene->font();
    font.setPointSize(10);

    viewportPadding = new QGraphicsRectItem();
    viewportPadding->setRect(0,0, barAmount * hScaleFactor, 5);
    viewportPadding->setPen(Qt::NoPen);
    scene->addItem(viewportPadding);

    // Timeline Numbers and Lines
    for (int i = 0; i < barAmount; i++)
    {
        QGraphicsTextItem *text = scene->addText(QString::number(i + 1), font);
        text->setPos((i * hScaleFactor) + 3, -3);
        QGraphicsLineItem *line = scene->addLine(QLine(0, 0, 0, 15), QColor("#0f0f0f"));
        line->setPos(i * hScaleFactor, 0);
        barLines->insert(barLines->end(), line);
        barNumbers->insert(barNumbers->end(), text);

    }

}

void RulerGraphicWidget::setHScaleFactor(int _value) {
    hScaleFactor = _value;
    for (int i = 0; i < barLines->length(); i++) {
        const int barPos = i * hScaleFactor;
        barLines->at(i)->setX(barPos);
        barNumbers->at(i)->setX(barPos  + 3);

    }
}


void RulerGraphicWidget::setBarAmount(int _value) {
    QFont font = scene->font();
    font.setPointSize(10);
    if (barAmount < _value) {

        for (int i = barAmount; i < _value; i++)
        {
            QGraphicsTextItem *text = scene->addText(QString::number(i + 1), font);
            text->setPos((i * hScaleFactor) + 3, -3);
            QGraphicsLineItem *line = scene->addLine(QLine(0, 0, 0, 15), QColor("#0f0f0f"));
            line->setPos(i * hScaleFactor, 0);
            barLines->insert(barLines->end(), line);
            barNumbers->insert(barNumbers->end(), text);
        }
        barAmount = _value;
    } else {
        barAmount = _value;
        for (int i = barAmount; i < barLines->size(); i++) {
            scene->removeItem(barLines->at(i));
            scene->removeItem(barNumbers->at(i));
            barLines->removeAt(i);
            barNumbers->removeAt(i);
        }
    }


}
RulerGraphicWidget::~RulerGraphicWidget()
{
}



