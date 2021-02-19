#ifndef HORIZONDIAL_H
#define HORIZONDIAL_H

#include <QDial>
#include <QString>
#include <QSharedPointer>

class HorizonDial : public QDial
{
    Q_OBJECT

    Q_PROPERTY(QString arcColor READ getArcColor WRITE setArcColor)

    Q_PROPERTY(double arcWidth READ getArcWidth WRITE setArcWidth)

    public:

        HorizonDial(const QString& suffix,
                    QWidget * parent = nullptr,
                    int minimum = 0,
                    int maximum = 999);

        void setArcColor(const QString& color);

            QString getArcColor() const;


            void setStartAngle(double angle);

            double getStartAngle() const;


            void setMaximumAngle(double angle);

            double getMaximumAngle() const;


            void setArcWidth(double px);

            double getArcWidth() const;


            void setSuffix(const QString& text);

            QString getSuffix() const;

            void updateSize();

    private slots:

    void updateValue();

    private:

    virtual void paintEvent(QPaintEvent*) override;

        virtual void resizeEvent(QResizeEvent* event) override;

        double maximumAngleSpan_;

        double startAngle_;

        double arcWidth_;

        double angleSpan_;

        QString valueString_;

        QString suffix_;

        QSharedPointer<QRectF> arcRect_;

        QSharedPointer<QRectF> valueRect_;

        QSharedPointer<QRectF> suffixRect_;

        QSharedPointer<QColor> arcColor_;

        QSharedPointer<QPen> arcPen_;
    };


#endif // HORIZONDIAL_H
