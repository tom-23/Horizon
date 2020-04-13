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


private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();


    void on_zoomSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

private:
    Ui::ArrangeWidget *ui;
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // ARRANGEWIDGET_H
