#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include "client.h"
#include "common/dialogs.h"
#include "app/audiomanager.h"
//#include "app/projectserialization.h"

#include "gui/mainwindow.h"
class MainWindow;
class ProjectSerialization;

class Session : public QObject
{
    Q_OBJECT

public:
    Session(QWidget *parent, AudioManager &_audioMan);
    QString getSessionUUID();

    bool getActive();
    bool getIsHost();

    void connect(QString _id, QString _password, QString _userUUID, bool isHost);
    void disconnectSession();
    void closeSession();

    void setCookies(QNetworkCookieJar *_cookieJar);

    void transferCurrentProject();
    void askForLatestProjectFile();

    void newTrack(QString colorHex, QString uuid);
    void moveRegion(QString uuid, double gridLocation);
    void setRegionTrack(QString regionUUID, QString trackUUID);

    void setTrackMute(QString uuid, bool mute);
    void setTrackPan(QString uuid, float pan);
    void setTrackGain(QString uuid, float gain);

    void uploadFile(QString fileName, QString hash);
    void downloadFile(QString url);

    void startUploads();
    void startDownloads();
private:
    QString id;
    QString password;
    QString sessionID;

    QString userUUID;
    QString sessionUUID;

    QString endPoint;

    bool isActive;
    bool isHost;

    Client *webSockClient;

    QNetworkAccessManager *netManager;
    QNetworkRequest netRequest;

    AudioManager* audioMan;

    void onSessionConnected();
    void onJSON(QJsonObject object);

    QTimer *heartbeatTimer;
    MainWindow *mainWindow;

    QList<QList<QString>> uploadQueue;
    QList<QString> downloadQueue;

    int filesUploaded;
    int filesDownloaded;

    std::function<void()> downloadCallback;

private slots:
    void netManagerFinished(QNetworkReply *reply);

    void heartbeat();
};

#endif // SESSION_H
