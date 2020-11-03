#include "rtcclientwindow.h"
#include "ui_rtcclientwindow.h"

RTCClientWindow::RTCClientWindow(QWidget *parent, Session *_session, UAC *_uac) :
    QDialog(parent),
    ui(new Ui::RTCClientWindow)
{
    ui->setupUi(this);
    session = _session;
    uac = _uac;
    uac->getUserUUID(this, std::bind(&RTCClientWindow::userUUIDCallback, this));
    ui->sessionID->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->sessionPassword->setAttribute(Qt::WA_MacShowFocusRect, 0);
}

void RTCClientWindow::userUUIDCallback() {
    if (uac->userUUID != "") {
        userUUID = uac-> userUUID;
        ui->logoutButton->setText("Logout (" + uac->name + ")");
        if (session != nullptr && session->getActive() == false) {
            debug::out(3, "All info seems satisfied.");
            //session->setSID(uac->sidCookie);
            session->setCookies(uac->cookieJar);
        }
    } else {
        this->close();
    }

}

RTCClientWindow::~RTCClientWindow()
{
    delete ui;
}

void RTCClientWindow::on_connectButton_clicked()
{
    session->connect(ui->sessionID->text(), ui->sessionPassword->text(), userUUID, false);
}

void RTCClientWindow::on_logoutButton_clicked()
{
    if (ui->logoutButton->text() == "Login") {
        uac->getUserUUID(this, std::bind(&RTCClientWindow::userUUIDCallback, this));
    } else {
        uac->logoutUser();
        this->close();
    }
}
