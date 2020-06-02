#ifndef ARRANGEWIDGET_H
#define ARRANGEWIDGET_H

#include <QWidget>
#include "trackcontrolswidget.h"

#include "timeline.h"


namespace Ui {
class ArrangeWidget;
}

class ArrangeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ArrangeWidget(QWidget *parent = nullptr);
    ~ArrangeWidget();

    Timeline *tl;


private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();


    void on_zoomSlider_valueChanged(int value);



    void on_pushButton_clicked();

    void on_trackRuler_sliderChange(int _value);

private:
    Ui::ArrangeWidget *ui;
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // ARRANGEWIDGET_H
