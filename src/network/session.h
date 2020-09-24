#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include "client.h"
#include "common/dialogs.h"


class Session : public QObject
{
    Q_OBJECT

public:
    Session(QWidget *parent = nullptr);;
    QString getSessionUUID();

    bool getActive();
    void connect(QString _id, QString _password, QString _userUUID);

    void setSID(const QNetworkCookie &_sidCookie);



private:
    QString id;
    QString password;
    QString sessionID;
    QString userUUID;
    bool isActive;

    Client *webSockClient;

    QNetworkAccessManager *netManager;
    QNetworkRequest netRequest;

    QNetworkCookieJar *cookieJar;
    const QNetworkCookie sidCookie;

private slots:
    void netManagerFinished(QNetworkReply *reply);
};

#endif // SESSION_H
