#include "infowidget.h"
#include "ui_infowidget.h"

InfoWidget::InfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoWidget)
{
    ui->setupUi(this);

    masterIcon.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/master.svg"), QSize(16,16));
    trackIcon.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/track.svg"), QSize(16,16));
    regionIcon.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/region.svg"), QSize(16,16));

    masterIconSelected.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/master_selected.svg"), QSize(16,16));
    trackIconSelected.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/track_selected.svg"), QSize(16,16));
    regionIconSelected.addFile(dialogs::getThemeManager()->colorizeSVG(":/svg/svg/16x/region_selected.svg"), QSize(16,16));

    resetIcons();

    ui->toolBox->setCurrentIndex(2);
}

InfoWidget::~InfoWidget()
{
    delete ui;
}

void InfoWidget::on_toolBox_currentChanged(int index)
{
    resetIcons();
    switch (index) {
        case 0:
            ui->toolBox->setItemIcon(0, masterIconSelected);
        break;
        case 1:
            ui->toolBox->setItemIcon(1, trackIconSelected);
        break;
        case 2:
            ui->toolBox->setItemIcon(2, regionIconSelected);
        break;
    }
}

void InfoWidget::resetIcons() {

    ui->toolBox->setItemIcon(0, masterIcon);
    ui->toolBox->setItemIcon(1, trackIcon);
    ui->toolBox->setItemIcon(2, regionIcon);
}
