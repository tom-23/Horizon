#include "librarywidget.h"
#include "ui_librarywidget.h"
#include <QtGui>
#include <QFileSystemModel>

LibraryWidget::LibraryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibraryWidget)
{
    ui->setupUi(this);

}

LibraryWidget::~LibraryWidget()
{
    delete ui;
}
