#ifndef CLIENT_H
#define CLIENT_H

#include <QtWebSockets/QtWebSockets>
#include "common/debug.h"


class Client : public QObject
{
    Q_OBJECT
public:
    Client(const QUrl &_url, bool _debug = false, QString token = "", QObject *_parent = nullptr, QString userUUID = "", std::function<void()> callback = nullptr, std::function<void(QJsonObject)> onMessage = nullptr);

    void authenticate();
    void sendCommand(QString cmnd = "blank", QList<QString> args = {});
    void sendCommandObject(QString cmnd, QJsonObject object);

    void sendJSONObject(QString type, QJsonObject object);

    QString getToken();

private Q_SLOTS:
    void onConnected();

    void onJSONMessageReceived(QString message);
Q_SIGNALS:
    void closed();

private:
        QWebSocket webSocket;
        QUrl url;
        QString token;
        QString userUUID;

        std::function<void()> callback;
        std::function<void(QJsonObject)> onMessage;
};

#endif // CLIENT_H
