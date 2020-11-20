#ifndef RTCCLIENTWINDOW_H
#define RTCCLIENTWINDOW_H

#include <QDialog>
#include "common/dialogs.h"
#include "mainwindow.h"

class UAC;
class Session;



namespace Ui {
class RTCClientWindow;
}

class RTCClientWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RTCClientWindow(QWidget *parent = nullptr, Session *_session = nullptr, UAC *_uac = nullptr);
    ~RTCClientWindow();

private slots:
    void on_connectButton_clicked();

    void on_logoutButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::RTCClientWindow *ui;

    Session *session;
    UAC *uac;

    QString userUUID;

    void userUUIDCallback();
};

#endif // RTCCLIENTWINDOW_H
