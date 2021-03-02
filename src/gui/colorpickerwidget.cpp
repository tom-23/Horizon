#include "colorpickerwidget.h"
#include "ui_colorpickerwidget.h"

// TODO: this feature needs to be implemented.

ColorPickerWidget::ColorPickerWidget(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::ColorPickerWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Tool | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    for (int var = 0; var < int(colorPallet.size()); ++var) {
        QWidget *colorButton = new QWidget(this);
        colorButton->setMinimumWidth(20);
        colorButton->setMinimumHeight(15);
        colorButton->setStyleSheet("background-color: " + colorPallet.at(var));
        ui->colorButtonsLayout->addWidget(colorButton);
        colorButton->show();
    }
    this->adjustSize();
}

ColorPickerWidget::~ColorPickerWidget()
{
    delete ui;
}


QColor ColorPickerWidget::getColor() {
    this->show();
    return QColor("#FFFFF");
}
