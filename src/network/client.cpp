#include "client.h"

#include <QtCore/QDebug>

QT_USE_NAMESPACE

//! [constructor]
Client::Client(const QUrl &_url, bool _debug, QString _token, QObject *_parent, QString _userUUID, std::function<void()> _callback, std::function<void(QJsonObject)> _onMessage ) :
    QObject(_parent),
    url(_url),
    token(_token),
    userUUID(_userUUID),
    callback(_callback),
    onMessage(_onMessage)
{

    // setup websockets stuff...
    debug::out(3, "Opening websockets connection...");
    connect(&webSocket, &QWebSocket::connected, this, &Client::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &Client::closed);
    connect(&webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
    [=](QAbstractSocket::SocketError error){
        qDebug() << error;
    });

    QNetworkRequest connectionRequest((QUrl(url)));
    // we set our auth token here to ensure we are authenticated
    connectionRequest.setRawHeader("token", token.toUtf8());
    webSocket.open(connectionRequest);
}

void Client::onConnected()
{
    debug::out(3, "Connected to websocket! Let's get ready to rumble!"); // lets get ready to RUMBLE!
    connect(&webSocket, &QWebSocket::textMessageReceived, this, &Client::onJSONMessageReceived);
    authenticate();
    sendCommand("ping");
    callback();
}

void Client::authenticate() {
    // why is this here?
    // TODO: remove this, it does nothing
}

void Client::sendCommand(QString cmnd, QList<QString> args) {
    QJsonDocument jsonDocument;
    QJsonObject root;
    // this handles commands, we type set the payload type to cmnd and set the payload.
    root.insert("type", "cmnd");
    root.insert("payload", cmnd);
    // for each argument, add it to the json...
    for (int i = 0; i < args.size(); i++) {
        root.insert("arg" + QString::number(i), args[i]);
    }
    // we need to insert our own userUUID to ensure we are not following out actions we've sent (we've already done)
    root.insert("userUUID", userUUID);

    jsonDocument.setObject(root);

    webSocket.sendTextMessage(jsonDocument.toJson());
}

void Client::sendJSONObject(QString type, QJsonObject object) {
    QJsonDocument jsonDocument;
    QJsonObject root;

    root.insert("type", type);
    root.insert("payload", object);
    root.insert("userUUID", userUUID);

    jsonDocument.setObject(root);

    webSocket.sendTextMessage(jsonDocument.toJson());
}

void Client::sendCommandObject(QString cmnd, QJsonObject object) {
    QJsonDocument jsonDocument;
    QJsonObject root;

    root.insert("type", "cmnd");
    root.insert("payload", cmnd);
    root.insert("args", object);
    root.insert("userUUID", userUUID);

    jsonDocument.setObject(root);

    webSocket.sendTextMessage(jsonDocument.toJson());
}

void Client::onJSONMessageReceived(QString message)
{
    // here we handle messages recieved. We parse the json then ensure its coming from someone else.
    QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject root = jsonDocument.object();
    qDebug() << "GOT JSON MESSAGE";
    qDebug() << "is the uuid the same? :" << (root.value("userUUID") != userUUID);
    if (root.value("userUUID") != userUUID) {
        onMessage(root);
    }


}

QString Client::getToken() {
    return token;
}
