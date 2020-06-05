#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QDesktopServices>
#include <QGraphicsItem>

AboutDialog::AboutDialog(QWidget *parent, ThemeManager *themeManager) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->imageContainer->setStyleSheet("image: url('" + themeManager->colorizeSVG(":/svg/svg/logo_base.svg") + "');");

    this->setModal(true);
    this->setWindowFlag(Qt::WindowCloseButtonHint);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl("http://tom-butcher.ml:2233/git/tom/Horizon"));
}

void AboutDialog::on_pushButton_2_clicked()
{
    this->close();
}
