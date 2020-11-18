#include "udstatuswindow.h"
#include "ui_udstatuswindow.h"

UDStatusWindow::UDStatusWindow(QWidget *parent, QList<QString> listOfItems) :
    QDialog(parent),
    ui(new Ui::UDStatusWindow)
{
    ui->setupUi(this);
    ui->listWidget->addItems(listOfItems);
    QString iconSVG = dialogs::getThemeManager()->colorizeSVG(":/svg/svg/sync.svg");
    ui->iconBox->setStyleSheet("image: url('" + iconSVG + "');");

}

UDStatusWindow::~UDStatusWindow()
{
    delete ui;
}

void UDStatusWindow::popTop() {
    ui->listWidget->takeItem(0);
}

void UDStatusWindow::updateMax(int value) {
    ui->progressBar->setMaximum(value);
}

void UDStatusWindow::updateProgress(int value) {
    ui->progressBar->setValue(value);
}
