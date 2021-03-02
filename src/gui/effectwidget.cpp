#include "effectwidget.h"
#include "ui_effectwidget.h"

// T

EffectWidget::EffectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EffectWidget)
{
    ui->setupUi(this);
}

EffectWidget::~EffectWidget()
{
    delete ui;
}
