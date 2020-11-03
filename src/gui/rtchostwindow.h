#ifndef RTCHOSTWINDOW_H
#define RTCHOSTWINDOW_H

#include <QDialog>
#include "common/dialogs.h"
#include "ui_rtchostwindow.h"
#include "mainwindow.h"

class UAC;
class Session;

namespace Ui {
class RTCHostWindow;
}

class RTCHostWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RTCHostWindow(QWidget *parent = nullptr, Session *_session = nullptr, UAC *_uac = nullptr);
    ~RTCHostWindow();

private slots:
    void on_logoutButton_clicked();
    void on_clientList_customContextMenuRequested(const QPoint &pos);

    void on_endSessionButton_clicked();

private:
    Ui::RTCHostWindow *ui;

    Session *session;
    UAC *uac;

    QString sessionUUID;
    QString userUUID;

    void userUUIDCallback();
    void createSessionCallback();

    void createSession();

};

#endif // RTCHOSTWINDOW_H
