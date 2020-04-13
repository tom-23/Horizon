#include "infowidget.h"
#include "ui_infowidget.h"

InfoWidget::InfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoWidget)
{
    ui->setupUi(this);
}

InfoWidget::~InfoWidget()
{
    delete ui;
}

void InfoWidget::addEffect(EffectWidget *effectWidget) {
    QVBoxLayout* effectLayout = qobject_cast<QVBoxLayout*>(ui->effectListContents->layout());
    effectLayout->insertWidget(effectLayout->count() - 1, effectWidget);
    effectWidget->show();
}
