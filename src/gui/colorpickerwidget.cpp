#include "colorpickerwidget.h"
#include "ui_colorpickerwidget.h"

ColorPickerWidget::ColorPickerWidget(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::ColorPickerWidget)
{
    ui->setupUi(this);
}

ColorPickerWidget::~ColorPickerWidget()
{
    delete ui;
}


QColor ColorPickerWidget::getColor() {
    this->show();
    return QColor("#FFFFF");
}
