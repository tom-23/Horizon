#include "colorpickerwidget.h"
#include "ui_colorpickerwidget.h"

ColorPickerWidget::ColorPickerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorPickerWidget)
{
    ui->setupUi(this);
}

ColorPickerWidget::~ColorPickerWidget()
{
    delete ui;
}
