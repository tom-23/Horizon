#include "uac.h"

UAC::UAC()
{
    sid = "";
    sidSet = false;
    QWebEngineProfile::defaultProfile()->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);

    webEng = new QWebEngineView();
    webEng->setFixedSize(550,500);
    webEng->setWindowFlags(Qt::Dialog);
    webEng->setWindowModality(Qt::WindowModal);
    webEng->setVisible(false);
    webCookieStore = QWebEngineProfile::defaultProfile()->cookieStore();
    connect(webCookieStore, &QWebEngineCookieStore::cookieAdded, this, &UAC::onWebEngineCookie);


    connect(webEng, &QWebEngineView::loadStarted, this, &UAC::onWebEngineViewLoadStarted);
    connect(webEng, &QWebEngineView::loadFinished, this, &UAC::onWebEngineViewLoaded);

    webEng->close();

    cookieJar = new QNetworkCookieJar();
}

UAC::~UAC() {
    debug::out(3, "Destruting Web Engine...");
    webEng->close();
}

void UAC::getUserUUID(QWidget *parent, std::function<void()> callback) {
    dialogParent = parent;
    uuidCallback = callback;
    debug::out(3, "Attempting to grab user info...");
    webEng->stop();
    webEng->load(QUrl(baseURI + "/users/getUserInfo"));
}

void UAC::logoutUser() {
    webEng->load(QUrl(baseURI + "/users/logout"));
    webEng->setVisible(false);
}

void UAC::createNewSession(std::function<void()> callback) {
    sessionCallback = callback;
    webEng->load(QUrl(baseURI + "/sessions/createSession"));
    webEng->setVisible(false);
}

void UAC::onWebEngineViewLoaded(bool ok) {
    dialogs::ProgressDialog::close();
    qDebug() << "URI" << webEng->url();

    if (ok) {
        webEng->page()->toPlainText([this](const QString& result) mutable {emit onWebDataRecieved(result);});
    } else {
        webEng->page()->toPlainText([this](const QString& result) mutable {emit onWebErrorRecieved(result);});
        webEng->setVisible(false);
        dialogs::MessageDialog::show("Error connecting to server",
                                 "There was an error connecting to the server. Please ensure that you have an internet connection and try again.",
                                 dialogs::MessageDialog::icons::no, dialogs::MessageDialog::buttons::okOnly);
        userUUID = "";
        uuidCallback();
    }
}

void UAC::onWebErrorRecieved(QString error) {
    qDebug() << "BODY" << error;
}

void UAC::onWebDataRecieved(QString data) {

     debug::out(3, "WebView Data Returned");
    QString endPoint = webEng->url().toString().split("/").at(webEng->url().toString().split("/").size() - 1);

    if (endPoint == "getUserInfo") {

        QJsonDocument responseJSON = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject *responseObject = new QJsonObject(responseJSON.object());
        webEng->setVisible(false);
        if (responseObject->value("loggedIn") == false) {

            debug::out(3, "Invalid SID. Logging in again...");
            webEng->load(QUrl(baseURI + "/users/login"));
        } else {
            debug::out(3, "Logged in, got a valid SID");
            name = responseObject->value("name").toString();
            email = responseObject->value("email").toString();
            userUUID = responseObject->value("uuid").toString();
            uuidCallback();


        }
    } else if (endPoint == "login") {
        webEng->setParent(dialogParent);
        webEng->setWindowFlags(Qt::Dialog);
        webEng->setWindowModality(Qt::WindowModal);
        webEng->setVisible(true);
        webEng->show();

    } else if (endPoint == "cancel") {
        debug::out(3, "Closing WebView...");
        userUUID = "";
        webEng->setVisible(false);
        uuidCallback();

    } else if (endPoint == "logout") {
        debug::out(3, "Set Logging Out...");
        sidSet = false;
        webEng->page()->profile()->cookieStore()->deleteAllCookies();
        webEng->load(QUrl(""));
        webEng->setVisible(false);

    } else if (endPoint == "createSession") {
        debug::out(3, "Creating new session...");
        QJsonDocument responseJSON = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject *responseObject = new QJsonObject(responseJSON.object());

        sessionUUID = responseObject->value("uuid").toString();
        sessionID = responseObject->value("connectionID").toString();
        sessionPassword = responseObject->value("password").toString();

        webEng->setVisible(false);
        sessionCallback();



     } else {

        debug::out(2, "Unknown Endpoint " + endPoint.toStdString());
    }
}
void UAC::onWebEngineViewLoadStarted() {
    dialogs::ProgressDialog::show(0, 0, "Loading...");
}

void UAC::onWebEngineCookie(const QNetworkCookie &cookie) {
    cookieJar->insertCookie(cookie);
}

