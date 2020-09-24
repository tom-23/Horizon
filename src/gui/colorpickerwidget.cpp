#include "colorpickerwidget.h"
#include "ui_colorpickerwidget.h"

ColorPickerWidget::ColorPickerWidget(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::ColorPickerWidget)
{
    ui->setupUi(this);
    for (int var = 0; var < int(colorPallet.size()); ++var) {
        QWidget *colorButton = new QWidget(this);
        colorButton->setMinimumWidth(20);
        colorButton->setMinimumHeight(15);
        colorButton->setStyleSheet("background-color: " + QString::fromStdString(colorPallet.at(var)));
        ui->colorButtonsLayout->addWidget(colorButton);
        colorButton->show();
    }
}

ColorPickerWidget::~ColorPickerWidget()
{
    delete ui;
}


QColor ColorPickerWidget::getColor() {
    this->show();
    return QColor("#FFFFF");
}
