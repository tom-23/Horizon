#include "session.h"

Session::Session(QWidget *parent, AudioManager &_audioMan)
    : QObject(parent)
{
    audioMan = &_audioMan;
    netManager = new QNetworkAccessManager(this);
    netRequest.setRawHeader("User-Agent", "Horizon RTC Client");
    netRequest.setRawHeader("X-Custom-User-Agent", "Horizon RTC Client");
    netRequest.setRawHeader("Content-Type", "application/json");

    isActive = false;
    mainWindow = qobject_cast<MainWindow *>(parent);

    QObject::connect(netManager, &QNetworkAccessManager::finished, this, &Session::netManagerFinished);

    heartbeatTimer = new QTimer(parent);
    QObject::connect(heartbeatTimer, &QTimer::timeout, this, QOverload<>::of(&Session::heartbeat));



}

QString Session::getSessionUUID() {
    return sessionID;
}

bool Session::getActive() {
    return isActive;
}

void Session::setCookies(QNetworkCookieJar *_cookieJar) {
    qDebug() << "Set session cookie jar";
    netManager->setCookieJar(_cookieJar);
    //netManager->cookieJar()->insertCookie(_sidCookie);
}
void Session::connect(QString _id, QString _password, QString _userUUID, bool _isHost) {

    id = _id;
    password = _password;
    userUUID = _userUUID;
    isHost = _isHost;

    dialogs::ProgressDialog::show(0,0,"Connecting to session...");

    QJsonDocument doc;
    QJsonObject root;
    root.insert("connectionID", id);
    root.insert("password", password);
    root.insert("userUUID", userUUID);
    doc.setObject(root);

    QByteArray postDataSize = QByteArray::number(doc.toJson().size());

    netRequest.setRawHeader("Content-Length", postDataSize);
    netRequest.setUrl(QUrl("http://127.0.0.1:3000/sessions/connectSession"));
    netManager->post(netRequest, doc.toJson());
}

void Session::netManagerFinished(QNetworkReply *reply) {
    dialogs::ProgressDialog::close();
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QString response = reply->readAll();
    QString endPoint = netRequest.url().toString().split("/").at(netRequest.url().toString().split("/").size() - 1);

    qDebug() << response.toUtf8();
    if (endPoint == "connectSession") {

        QJsonDocument responseJSON = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject *responseObject = new QJsonObject(responseJSON.object());

        qDebug() << "TOKEN" << responseObject->value("token").toString();
        netRequest.setRawHeader("SessionToken", responseObject->value("token").toString().toUtf8());

        if (responseObject->value("result").toString() == "ok") {
            debug::out(3, "Got connection token, setting up websockets...");
            sessionUUID = responseObject->value("uuid").toString();
            webSockClient = new Client(QUrl("ws://127.0.0.1:3001/ws-session/" + responseObject->value("uuid").toString())
                                       , false
                                       , responseObject->value("token").toString()
                                       , this
                                       , userUUID
                                       , std::bind(&Session::onSessionConnected, this)
                                       , std::bind(&Session::onJSON, this, std::placeholders::_1));

        } else if (responseObject->value("result").toString() == "error") {
            dialogs::MessageDialog::show(responseObject->value("reason").toString().toStdString()
                                         , "Couldn't connect to session. Please make sure the details you entered are correct."
                                         , dialogs::MessageDialog::icons::caution
                                         , dialogs::MessageDialog::buttons::okOnly);
        }
    }
}

void Session::onSessionConnected() {
    isActive = true;
    if (!isHost) {
        dialogs::MessageDialog::show("Connected to session!"
                                     , "Have fun making making some music ;)"
                                     , dialogs::MessageDialog::icons::info
                                     , dialogs::MessageDialog::buttons::okOnly);
        askForLatestProjectFile();
    } else {
        transferCurrentProject();
    }

}

void Session::transferCurrentProject() {
    ProjectSerialization *projSer = new ProjectSerialization();
    QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(projSer->serialize(*audioMan, true)).toUtf8());
    webSockClient->sendJSONObject("project", doc.object());
    qDebug() << QStandardPaths::writableLocation(QStandardPaths::TempLocation);
}

void Session::askForLatestProjectFile() {
    webSockClient->sendCommand("latestProject");
}

void Session::heartbeat() {
    qDebug() << "BaBum";
}

void Session::onJSON(QJsonObject object) {
    QString type = object.value("type").toString();

    if (type == "project") {
        QJsonDocument doc;
        QJsonObject project = object.value("payload").toObject();
        doc.setObject(project);
        qDebug() << "Calling load project JSON";
        mainWindow->loadProjectJSON(doc.toJson());
    } else if (type == "cmnd") {
        QString payload = object.value("payload").toString();
        QJsonObject args = object.value("args").toObject();

        if (payload == "newTrack") {
            qDebug() << "Adding new track";
            QColor color;
            color.setNamedColor(args.value("color").toString());
            mainWindow->newTrack(color, args.value("trackUUID").toString());

        } else if (payload == "moveRegion") {
            qDebug() << "Moving region";
            mainWindow->moveRegion(args.value("regionUUID").toString(), args.value("gridLocation").toDouble());
        } else if (payload == "getProject") {
            qDebug() << "Got request for project";
            transferCurrentProject();
        }
    }
}

void Session::newTrack(QString colorHex, QString uuid) {
    if (isActive) {
        qDebug() << "Sending new track command";
        QJsonObject root;
        root.insert("trackUUID", uuid);
        root.insert("color", colorHex);
        webSockClient->sendCommandObject("newTrack", root);
    }
}

void Session::moveRegion(QString uuid, double gridLocation) {
    if (isActive) {
        QJsonObject root;
        root.insert("regionUUID", uuid);
        root.insert("gridLocation", gridLocation);
        webSockClient->sendCommandObject("moveRegion", root);
    }
}

void Session::setRegionTrack(QString regionUUID, QString trackUUID) {
    if (isActive) {
        qDebug() << "Sending region track command";
        QJsonObject root;
        root.insert("trackUUID", trackUUID);
        root.insert("regionUUID", regionUUID);
        webSockClient->sendCommandObject("setRegionTrack", root);
    }
}

void Session::setTrackMute(QString uuid, bool mute) {
    if (isActive) {
        qDebug() << "Sending track mute command";
        QJsonObject root;
        root.insert("trackUUID", uuid);
        root.insert("mute", mute);
        webSockClient->sendCommandObject("setTrackMute", root);
    }
}

void Session::closeSession() {
    if (getActive() == true) {
        QJsonDocument doc;
        QJsonObject root;
        root.insert("userUUID", userUUID);
        root.insert("sessionUUID", sessionUUID);
        doc.setObject(root);

        QByteArray postDataSize = QByteArray::number(doc.toJson().size());

        netRequest.setRawHeader("Content-Length", postDataSize);
        netRequest.setUrl(QUrl("http://192.168.1.231:3000/sessions/deleteSession"));
        netManager->post(netRequest, doc.toJson());
        delete webSockClient;
        isActive = false;
    }
}
