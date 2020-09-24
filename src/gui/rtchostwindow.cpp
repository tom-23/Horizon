#include "rtchostwindow.h"
#include "ui_rtchostwindow.h"

RTCHostWindow::RTCHostWindow(QWidget *parent, Session *_session) :
    QDialog(parent),
    ui(new Ui::RTCHostWindow)
{
    ui->setupUi(this);
    session = _session;
    QWebEngineProfile::defaultProfile()->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);

    debug::out(3, "Loading WebEngineView...");
    webEng = new QWebEngineView(this);
    webEng->setFixedSize(550,500);
    webEng->setWindowFlags(Qt::Dialog);
    webEng->setWindowModality(Qt::WindowModal);
    webEng->setVisible(false);
    webCookieStore = QWebEngineProfile::defaultProfile()->cookieStore();
    connect(webEng, &QWebEngineView::loadStarted, this, &RTCHostWindow::onWebEngineViewLoadStarted);
    connect(webEng, &QWebEngineView::loadFinished, this, &RTCHostWindow::onWebEngineViewLoaded);
    connect(webCookieStore, &QWebEngineCookieStore::cookieAdded, this, &RTCHostWindow::onWebEngineCookie);

    debug::out(3, "Loaded WebEngineView, getting user info...");

    webEng->load(QUrl("http://127.0.0.1:3000/users/getUserInfo"));


}

void RTCHostWindow::onWebEngineViewLoaded(bool ok) {
    dialogs::ProgressDialog::close();

    if (ok) {
        webEng->page()->toPlainText([this](const QString& result) mutable {emit onWebDataRecieved(result);});
    } else {
        webEng->setVisible(false);
        dialogs::MessageDialog::show("Error connecting to server",
                                 "There was an error connecting to the server. Please ensure that you have an internet connection and try again.",
                                 dialogs::MessageDialog::icons::no, dialogs::MessageDialog::buttons::okOnly);
        this->close();
    }
}



void RTCHostWindow::onWebDataRecieved(QString data) {

     debug::out(3, "WebView Data Returned");
    QString endPoint = webEng->url().toString().split("/").at(webEng->url().toString().split("/").size() - 1);

    if (endPoint == "getUserInfo") {

        QJsonDocument responseJSON = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject *responseObject = new QJsonObject(responseJSON.object());

        webEng->setVisible(false);


        if (responseObject->value("loggedIn") == false) {

            debug::out(3, "Invalid SID. Logging in again...");
            webEng->load(QUrl("http://127.0.0.1:3000/users/login"));
            webEng->setVisible(true);
            webEng->show();

        } else {

            debug::out(3, "Logging in using SID...");
            ui->logoutButton->setEnabled(true);
            ui->logoutButton->setText("Logout (" + responseObject->value("name").toString() + ")");

            if (sessionUUID == "") {
                createSession();
            } else {

            }
        }
    } else if (endPoint == "login") {

    } else if (endPoint == "cancel") {
        debug::out(3, "Closing WebView...");
        webEng->close();
        webEng->setVisible(false);

    } else if (endPoint == "logout") {
        debug::out(3, "Set Logging Out...");
        webEng->close();
        this->close();

    } else if (endPoint == "createSession") {
        debug::out(3, "Creating new session...");
        QJsonDocument responseJSON = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject *responseObject = new QJsonObject(responseJSON.object());

        sessionUUID = responseObject->value("uuid").toString();
        ui->sessionIDLabel->setText(responseObject->value("connectionID").toString());
        ui->sessionPasswordLabel->setText(responseObject->value("password").toString());

        session->connect(responseObject->value("connectionID").toString(), responseObject->value("password").toString(), "");
        webEng->setVisible(false);



     } else {

        debug::out(2, "Unknown Endpoint " + endPoint.toStdString());
    }
}
void RTCHostWindow::onWebEngineViewLoadStarted() {
    dialogs::ProgressDialog::show(0, 0, "Loading...");
}

RTCHostWindow::~RTCHostWindow()
{
    delete ui;
}

void RTCHostWindow::on_logoutButton_clicked()
{
    if (ui->logoutButton->text() == "Login") {
        webEng->load(QUrl("http://127.0.0.1:3000/users/login"));
        webEng->setVisible(true);
        webEng->show();
    } else {
        webEng->load(QUrl("http://127.0.0.1:3000/users/logout"));
        webEng->setVisible(false);
    }

}


void RTCHostWindow::createSession() {
    webEng->load(QUrl("http://127.0.0.1:3000/sessions/createSession"));

    webEng->setVisible(false);
}


void RTCHostWindow::onWebEngineCookie(const QNetworkCookie &cookie) {
    if (cookie.name() == "connect.sid") {
        session->setSID(cookie);
        debug::out(3, "Set SID Cookie");
    }
}
