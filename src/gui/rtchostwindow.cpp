#include "rtchostwindow.h"
#include "ui_rtchostwindow.h"

RTCHostWindow::RTCHostWindow(QWidget *parent, Session *_session, UAC *_uac) :
    QDialog(parent),
    ui(new Ui::RTCHostWindow)
{
    ui->setupUi(this);
    session = _session;
    uac = _uac;

    uac->getUserUUID(this, std::bind(&RTCHostWindow::userUUIDCallback, this));

}

void RTCHostWindow::userUUIDCallback() {
    if (uac->userUUID != "") {
        userUUID = uac->userUUID;
        ui->logoutButton->setText("Logout (" + uac->name + ")");
        if (session != nullptr) {
            if (session->getActive() == false) {
                createSession();
            } else {
                ui->sessionIDLabel->setText(uac->sessionID);
                ui->sessionPasswordLabel->setText(uac->sessionPassword);
            }

        }
    } else {
        this->close();
    }

}

void RTCHostWindow::createSession() {
    debug::out(3, "Starting session...");
    session->setCookies(uac->cookieJar);
    uac->createNewSession(std::bind(&RTCHostWindow::createSessionCallback, this));
}



RTCHostWindow::~RTCHostWindow()
{
    delete ui;
}

void RTCHostWindow::on_logoutButton_clicked()
{
    if (ui->logoutButton->text() == "Login") {
        uac->getUserUUID(this, std::bind(&RTCHostWindow::userUUIDCallback, this));
    } else {
        uac->logoutUser();
        this->close();
    }

}


void RTCHostWindow::createSessionCallback() {
    ui->sessionIDLabel->setText(uac->sessionID);
    ui->sessionPasswordLabel->setText(uac->sessionPassword);
    session->connect(uac->sessionID, uac->sessionPassword, uac->userUUID, true);
    ui->endSessionButton->setText("End Session");
}




void RTCHostWindow::on_clientList_customContextMenuRequested(const QPoint &pos)
{
    if (ui->clientList->selectedItems().size() != 0) {
       QMenu menu(this);
       menu.setAutoFillBackground(true);

       QAction *kickClient = new QAction("Kick Client", this);
       menu.addAction(kickClient);

       //connect(kickClient, &QAction::triggered, this, &TrackControlsWidget::changeColor);
       menu.setWindowFlags(menu.windowFlags() | Qt::CustomizeWindowHint);
       menu.exec(mapToGlobal(pos));
    }
}

void RTCHostWindow::on_endSessionButton_clicked()
{
    if (ui->endSessionButton->text() == "End Session") {
        if (session->getActive() == true) {
            if (dialogs::MessageDialog::show("Session currently active"
                                             ,"Are you sure you want to end the current session?"
                                             , dialogs::MessageDialog::caution
                                             , dialogs::MessageDialog::yesNo) == 2) {
                session->closeSession();
                ui->sessionIDLabel->setText("------");
                ui->sessionPasswordLabel->setText("----");
                ui->endSessionButton->setText("New Session");
            }
        } else {
            ui->sessionIDLabel->setText("------");
            ui->sessionPasswordLabel->setText("----");
            ui->endSessionButton->setText("New Session");
        }
    } else {
        if (userUUID != "") {
            if (session != nullptr) {
                if (session->getActive() == false) {
                    createSession();
                }

            }
        }
    }

}
