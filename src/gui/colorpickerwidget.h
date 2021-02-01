#ifndef COLORPICKERWIDGET_H
#define COLORPICKERWIDGET_H

#include <QWidget>
#include "common/util.h"
#include <QList>
#include <QString>

namespace Ui {
class ColorPickerWidget;
}

class ColorPickerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPickerWidget(QWidget *parent = nullptr);
    ~ColorPickerWidget();

    QColor getColor();

    QList<QString> colorPallet = {"#FF5733", "#FFD133" ,"#90FF33", "#33FF58", "#33FFE3", "#33AFFF" ,"#3C33FF" ,"#A533FF" ,"#F633FF" ,"#FF3396" ,"#FF336B"};

private:
    Ui::ColorPickerWidget *ui;
};

#endif // COLORPICKERWIDGET_H
