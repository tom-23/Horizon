#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QGraphicsItem>

AboutDialog::AboutDialog(QWidget *parent, ThemeManager *themeManager) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->imageContainer->setStyleSheet("image: url('" + themeManager->colorizeSVG(":/svg/svg/logo_base.svg") + "');");


}

AboutDialog::~AboutDialog()
{
    delete ui;
}
