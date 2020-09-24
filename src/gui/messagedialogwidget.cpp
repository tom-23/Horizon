#include "messagedialogwidget.h"
#include "ui_messagedialogwidget.h"

MessageDialogWidget::MessageDialogWidget(QWidget *parent, QString title, QString message, QString svgPath, bool okOnly, bool okCancel, bool yesNo, bool yesNoCancel) :
    QDialog(parent),
    ui(new Ui::MessageDialogWidget)
{
    ui->setupUi(this);
    ui->titleLabel->setText(title);
    ui->messageLabel->setText(message);

    ui->iconBox->setStyleSheet("image: url('" + svgPath + "');");

    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    if (okOnly == true || okCancel == true) {
        QPushButton *okButton = new QPushButton(this);
        okButton->setText("&OK");
        okButton->setObjectName("okButton");
        okButton->setDefault(true);
        okButton->connect(okButton, &QPushButton::clicked, [this]() {
            this->accept();
        });
        ui->buttonLayout->addWidget(okButton);


    } else if (yesNo == true || yesNoCancel == true) {
        QPushButton *yesButton = new QPushButton(this);
        yesButton->setText("&Yes");
        yesButton->setObjectName("yesButton");
        yesButton->setDefault(true);
        yesButton->connect(yesButton, &QPushButton::clicked, [this]() {
            this->done(2);
        });
        ui->buttonLayout->addWidget(yesButton);

        QPushButton *noButton = new QPushButton(this);
        noButton->setText("&No");
        noButton->setObjectName("noButton");
        noButton->connect(noButton, &QPushButton::clicked, [this]() {
            this->done(3);
        });
        ui->buttonLayout->addWidget(noButton);
    }

    if (yesNoCancel == true || okCancel == true) {
            QPushButton *cancelButton = new QPushButton(this);
            cancelButton->setText("&Cancel");
            cancelButton->setObjectName("cancelButton");
            cancelButton->connect(cancelButton, &QPushButton::clicked, [this]() {
                this->reject();
            });
            ui->buttonLayout->addWidget(cancelButton);
    }

}

MessageDialogWidget::~MessageDialogWidget()
{
    delete ui;

}
