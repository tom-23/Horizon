#ifndef CLIENT_H
#define CLIENT_H

#include <QtWebSockets/QtWebSockets>
#include "common/debug.h"


class Client : public QObject
{
    Q_OBJECT
public:
    Client(const QUrl &_url, bool _debug = false, QString token = "", QObject *_parent = nullptr);

    void authenticate();

private Q_SLOTS:
    void onConnected();

    void onJSONMessageReceived(QString message);
Q_SIGNALS:
    void closed();
private:
        QWebSocket webSocket;
        QUrl url;
        QString token;
};

#endif // CLIENT_H
