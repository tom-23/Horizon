#include "client.h"

#include <QtCore/QDebug>

QT_USE_NAMESPACE

//! [constructor]
Client::Client(const QUrl &_url, bool _debug, QString _token, QObject *_parent) :
    QObject(_parent),
    url(_url),
    token(_token)
{
    debug::out(3, "Opening websockets connection...");
    connect(&webSocket, &QWebSocket::connected, this, &Client::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &Client::closed);

    QNetworkRequest connectionRequest((QUrl(url)));
    connectionRequest.setRawHeader("Authorization", token.toUtf8());
    webSocket.open(connectionRequest);
}

void Client::onConnected()
{
    debug::out(3, "Connected to websocket! Sending auth token...");
    connect(&webSocket, &QWebSocket::textMessageReceived, this, &Client::onJSONMessageReceived);
    authenticate();
}

void Client::authenticate() {
    QString payload = QString("{ \"type\": \"auth\", \"payload\": { \"token\": \"%1\"} }").arg(token);
    webSocket.sendTextMessage(payload);
}

void Client::onJSONMessageReceived(QString message)
{
    debug::out(3, "Recieved websocket data");
    qDebug() << message;
}

