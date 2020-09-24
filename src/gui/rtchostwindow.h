#ifndef RTCHOSTWINDOW_H
#define RTCHOSTWINDOW_H

#include <QDialog>
#include "common/dialogs.h"
#include "network/session.h"
#include "ui_rtchostwindow.h"
#include <QtWebEngineWidgets/QtWebEngineWidgets>

namespace Ui {
class RTCHostWindow;
}

class RTCHostWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RTCHostWindow(QWidget *parent = nullptr, Session *_session = nullptr);
    ~RTCHostWindow();

private slots:
    void on_logoutButton_clicked();
    void createSession();

private:
    Ui::RTCHostWindow *ui;

    QWebEngineView *webEng;
    QWebEngineCookieStore *webCookieStore;

    void onWebEngineViewLoaded(bool ok);
    void onWebEngineViewLoadStarted();
    void onWebEngineViewClosed();

    void onWebDataRecieved(QString data);
    void onWebEngineCookie(const QNetworkCookie &cookie);

    Session *session;

    QString sessionUUID;

};

#endif // RTCHOSTWINDOW_H
