#include "reverbfxwindow.h"
#include "ui_reverbfxwindow.h"

ReverbFXWindow::ReverbFXWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReverbFXWindow)
{
    ui->setupUi(this);
}

ReverbFXWindow::~ReverbFXWindow()
{
    delete ui;
}
