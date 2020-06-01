#ifndef COLORPICKERWIDGET_H
#define COLORPICKERWIDGET_H

#include <QWidget>

namespace Ui {
class ColorPickerWidget;
}

class ColorPickerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPickerWidget(QWidget *parent = nullptr);
    ~ColorPickerWidget();

private:
    Ui::ColorPickerWidget *ui;
};

#endif // COLORPICKERWIDGET_H
