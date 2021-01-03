#ifndef HORIZONDIAL_H
#define HORIZONDIAL_H

#include <QObject>
#include <QWidget>
#include <QDial>
#include <QDebug>
#include <QGraphicsItem>

class HorizonDial : public QDial
{
    Q_OBJECT

        Q_PROPERTY(double knobRadius READ getKnobRadius WRITE setKnobRadius);
        Q_PROPERTY(double knobMargin READ getKnobMargin WRITE setKnobMargin);
        Q_PROPERTY(QPixmap texture READ getTexture WRITE setTexture);

    public:

        HorizonDial(QWidget * parent = nullptr,
                   double knobRadius = 5,
                   double knobMargin = 5);

        void setKnobRadius(double radius);

        double getKnobRadius() const;

        void setKnobMargin(double margin);

        double getKnobMargin() const;

        QPixmap getTexture() const;

        void setTexture(QPixmap texture);

    private:

        virtual void paintEvent(QPaintEvent*) override;

        double knobRadius_;

        double knobMargin_;

        QPixmap texture_;
    };


#endif // HORIZONDIAL_H
