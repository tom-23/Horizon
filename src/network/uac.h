#ifndef UAC_H
#define UAC_H

#include <QObject>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include "common/dialogs.h"

class UAC : public QObject
{
    Q_OBJECT
public:
    UAC();
    ~UAC();
    void getUserUUID(QWidget *parent, std::function<void()> callback);
    void createNewSession(std::function<void()> callback);
    void logoutUser();

    QString name;
    QString email;
    QString userUUID;

    QString sessionUUID;
    QString sessionID;
    QString sessionPassword;

    QNetworkCookie sidCookie;

    QNetworkCookieJar *cookieJar;
private:

    QString sid;
    bool sidSet;
    QString baseURI = "http://127.0.0.1:3000";

    QWidget *dialogParent;

    std::function<void()> uuidCallback;
    std::function<void()> sessionCallback;

    QWebEngineView *webEng;
    QWebEngineCookieStore *webCookieStore;

    void onWebEngineViewLoaded(bool ok);
    void onWebEngineViewLoadStarted();
    void onWebEngineViewClosed();

    void onWebDataRecieved(QString data);
    void onWebErrorRecieved(QString error);
    void onWebEngineCookie(const QNetworkCookie &cookie);
};

#endif // UAC_H
