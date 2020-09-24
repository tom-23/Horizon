#include "session.h"

Session::Session(QWidget *parent)
    : QObject(parent)
{
    netManager = new QNetworkAccessManager(this);
    QObject::connect(netManager, &QNetworkAccessManager::finished, this, &Session::netManagerFinished);
}

QString Session::getSessionUUID() {
    return sessionID;
}

bool Session::getActive() {



    netManager->get(netRequest);
    return isActive;
}

void Session::setSID(const QNetworkCookie &_sidCookie) {


    netManager->cookieJar()->insertCookie(_sidCookie);
}
void Session::connect(QString _id, QString _password, QString _userUUID) {

    id = _id;
    password = _password;
    userUUID = _userUUID;

    dialogs::ProgressDialog::show(0,0,"Connecting to session...");

    QString body = QString("{ \"connectionID\":\"%1\", \"password\":\"%2\", \"userUUID\":\"%3\" }").arg(id).arg(password).arg(userUUID);

    qDebug() << body;

    QByteArray jsonString = body.toUtf8();
    QByteArray postDataSize = QByteArray::number(jsonString.size());

    netRequest.setRawHeader("User-Agent", "Horizon RTC Client");
    netRequest.setRawHeader("X-Custom-User-Agent", "Horizon RTC Client");
    netRequest.setRawHeader("Content-Type", "application/json");
    netRequest.setRawHeader("Content-Length", postDataSize);

    netRequest.setUrl(QUrl("http://127.0.0.1:3000/sessions/connectSession"));
    netManager->post(netRequest, jsonString);


}

void Session::netManagerFinished(QNetworkReply *reply) {
    dialogs::ProgressDialog::close();
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QString response = reply->readAll();
    QString endPoint = netRequest.url().toString().split("/").at(netRequest.url().toString().split("/").size() - 1);

    qDebug() << endPoint;
    if (endPoint == "connectSession") {

        QJsonDocument responseJSON = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject *responseObject = new QJsonObject(responseJSON.object());

        if (responseObject->value("result").toString() == "ok") {
            debug::out(3, "Got connection token, setting up websockets...");
            webSockClient = new Client(QUrl("ws://127.0.0.1:3000/sessions/sessionSocket/" + responseObject->value("uuid").toString()), false, responseObject->value("token").toString(), this);

        }
    }




}

