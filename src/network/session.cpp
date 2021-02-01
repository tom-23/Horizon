#include "session.h"

Session::Session(QWidget *parent, AudioManager &_audioMan)
    : QObject(parent)
{
    audioMan = &_audioMan;
    netManager = new QNetworkAccessManager(this);
    netRequest.setRawHeader("User-Agent", "Horizon RTC Client");
    netRequest.setRawHeader("X-Custom-User-Agent", "Horizon RTC Client");
    netRequest.setRawHeader("Content-Type", "application/json");

    isActive = false;
    mainWindow = qobject_cast<MainWindow *>(parent);

    QObject::connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(netManagerFinished(QNetworkReply*)));

    heartbeatTimer = new QTimer(parent);
    QObject::connect(heartbeatTimer, &QTimer::timeout, this, QOverload<>::of(&Session::heartbeat));

}

QString Session::getSessionUUID() {
    return sessionID;
}

bool Session::getActive() {
    return isActive;
}

bool Session::getIsHost() {
    return isHost;
}

void Session::setCookies(QNetworkCookieJar *_cookieJar) {
    qDebug() << "Set session cookie jar";
    netManager->setCookieJar(_cookieJar);
    //netManager->cookieJar()->insertCookie(_sidCookie);
}
void Session::connect(QString _id, QString _password, QString _userUUID, bool _isHost) {

    id = _id;
    password = _password;
    userUUID = _userUUID;
    isHost = _isHost;

    dialogs::ProgressDialog::show(0,0,"Connecting to session...");

    QJsonDocument doc;
    QJsonObject root;
    root.insert("connectionID", id);
    root.insert("password", password);
    root.insert("userUUID", userUUID);
    doc.setObject(root);

    QByteArray postDataSize = QByteArray::number(doc.toJson().size());

    netRequest.setRawHeader("Content-Length", postDataSize);
    endPoint = "connectSession";
    netRequest.setUrl(QUrl("https://horizon-rtc.systems/sessions/connectSession"));
    netManager->post(netRequest, doc.toJson());
}

void Session::netManagerFinished(QNetworkReply *reply) {

    if (reply->error()) {
        dialogs::ProgressDialog::close();
        qDebug() << reply->errorString();
        return;
    }

    //QByteArray response = reply->readAll();
    //QString endPoint = netRequest.url().toString().split("/").at(netRequest.url().toString().split("/").size() - 1);

    if (endPoint == "connectSession") {

        dialogs::ProgressDialog::close();
        QJsonDocument responseJSON = QJsonDocument::fromJson(reply->readAll());
        QJsonObject *responseObject = new QJsonObject(responseJSON.object());

        qDebug() << "TOKEN" << responseObject->value("token").toString();
        netRequest.setRawHeader("SessionToken", responseObject->value("token").toString().toUtf8());

        if (responseObject->value("result").toString() == "ok") {
            debug::out(3, "Got connection token, setting up websockets...");
            sessionUUID = responseObject->value("uuid").toString();
            webSockClient = new Client(QUrl("wss://horizon-rtc.systems/ws-session/" + responseObject->value("uuid").toString())
                                       , false
                                       , responseObject->value("token").toString()
                                       , this
                                       , userUUID
                                       , std::bind(&Session::onSessionConnected, this)
                                       , std::bind(&Session::onJSON, this, std::placeholders::_1));

        } else if (responseObject->value("result").toString() == "error") {
            dialogs::MessageDialog::show(responseObject->value("reason").toString().toStdString()
                                         , "Couldn't connect to session. Please make sure the details you entered are correct."
                                         , dialogs::MessageDialog::icons::caution
                                         , dialogs::MessageDialog::buttons::okOnly);
        }
    } else if (endPoint == "upload") {
        debug::out(3, "File Uploaded!");
        filesUploaded = filesUploaded + 1;
        udStatusWindow->popTop();
        if (filesUploaded == uploadQueue.size()) {
            uploadQueue.clear();
            udStatusWindow->close();
            debug::out(3, "All files uploaded successfully");
        } else {
            QList<QString> item = uploadQueue.at(filesUploaded);
            uploadFile(item.at(0), item.at(1));
        }
    } else if (endPoint == "download") {
        debug::out(3, "File Downloaded!");

       // QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/com.horizon.horizon";

        QString dirPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Horizon/" + downloadQueue.at(filesDownloaded).split("/")[1] + "/" + downloadQueue.at(filesDownloaded).split("/")[2];
        QDir dir(dirPath);
        dir.mkpath(dirPath);
        QString filename = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Horizon" + downloadQueue.at(filesDownloaded);


        if(!dir.exists() && !dir.mkpath(dirPath)) {
            qDebug() << "Error: can't create folder '"<< dirPath <<"'.";
        } else {
            qDebug() << "Folder '"<< dirPath <<"' exists or created successfully";
        }

        //qDebug() << "/com.horizon.horizon/" + downloadQueue.at(filesDownloaded).split("/")[1] + "/" + downloadQueue.at(filesDownloaded).split("/")[2];
        qDebug() << "FILE NAME" << filename;
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            debug::out(1, "Could not download file from server.");
        }
        QByteArray resp = reply->readAll();
        qDebug() << "STATUS CODE" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        file.write(resp);
        file.close();

        filesDownloaded = filesDownloaded + 1;
        udStatusWindow->popTop();
        if (filesDownloaded == downloadQueue.size()) {
            downloadQueue.clear();
            udStatusWindow->close();
            debug::out(3, "All files downloaded successfully");
            downloadCallback();
        } else {
            downloadFile(downloadQueue.at(filesDownloaded));
        }

    }
}

void Session::onSessionConnected() {
    isActive = true;
    if (!isHost) {
        dialogs::MessageDialog::show("Connected to session!"
                                     , "Have fun making making some music ;)"
                                     , dialogs::MessageDialog::icons::info
                                     , dialogs::MessageDialog::buttons::okOnly);
        askForLatestProjectFile();
    } else {
        transferCurrentProject();
    }

}

void Session::transferCurrentProject() {
    ProjectSerialization *projSer = new ProjectSerialization();
    projSer->sessionID = sessionUUID;
    projSer->copyToTemp = true;

    QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(projSer->serialize(*audioMan, true)).toUtf8());

    for (int i = 0; i < projSer->tempFileList.size(); i++) {
        qDebug() << i;
        uploadQueue.append(projSer->tempFileList.at(i));
    }

    if (uploadQueue.size() != 0) {
        startUploads();
    }



    webSockClient->sendJSONObject("project", doc.object());

}

void Session::askForLatestProjectFile() {

    webSockClient->sendCommand("latestProject");
}

void Session::heartbeat() {
    qDebug() << "BaBum";
}

void Session::onJSON(QJsonObject object) {
    QString type = object.value("type").toString();

    if (type == "project") {
        downloadQueue = {};
        QJsonDocument doc;
        QJsonObject project = object.value("payload").toObject();
       // QList<QString> fileArray = {};
        QJsonArray tracks = project.value("tracks").toArray();
        for (int i = 0; i < tracks.size(); i++) {
            QJsonArray regions = tracks.at(i).toObject().value("audioRegions").toArray();
            for (int r = 0; r < regions.size(); r++ ) {
                bool exists = false;
                QString fileName = regions.at(r).toObject().value("filePath").toString();
                for (int fn = 0; fn < downloadQueue.size(); fn++) {
                    if (downloadQueue.at(fn) == fileName) {
                        exists = true;
                    }
                }
                if (!exists) {
                    downloadQueue.append(fileName);
                    qDebug() << fileName;
                }
            }
        }
        doc.setObject(project);

        downloadCallback = [this, doc] {
            mainWindow->loadProjectJSON(doc.toJson());
        };
        if (downloadQueue.size() != 0) {
            startDownloads();
        } else {
            downloadCallback();
        }

        qDebug() << "Calling load project JSON";

    } else if (type == "cmnd") {
        QString payload = object.value("payload").toString();
        QJsonObject args = object.value("args").toObject();

        if (payload == "newTrack") {
            qDebug() << "Adding new track";
            QColor color;
            color.setNamedColor(args.value("color").toString());
            mainWindow->newTrack(color, args.value("trackUUID").toString());

        } else if (payload == "moveRegion") {
            qDebug() << "Moving region";
            audioMan->moveRegion(args.value("regionUUID").toString(), args.value("gridLocation").toDouble());
        } else if (payload == "getProject") {
            qDebug() << "Got request for project";
            transferCurrentProject();
        } else if (payload == "setTrackMute") {
            qDebug() << "Got repuest for track mute";
            audioMan->setTrackMute(args.value("trackUUID").toString(), args.value("mute").toBool());
        } else if (payload == "setTrackPan") {
            qDebug() << "Got repuest for track pan";
            audioMan->setTrackPan(args.value("trackUUID").toString(), args.value("pan").toDouble());
        } else if (payload == "setTrackGain") {
            qDebug() << "Got request for track gain";
            audioMan->setTrackGain(args.value("trackUUID").toString(), args.value("gain").toDouble());
        }
    }
}

void Session::newTrack(QString colorHex, QString uuid) {
    if (isActive) {
        qDebug() << "Sending new track command";
        QJsonObject root;
        root.insert("trackUUID", uuid);
        root.insert("color", colorHex);
        webSockClient->sendCommandObject("newTrack", root);
    }
}

void Session::moveRegion(QString uuid, double gridLocation) {
    if (isActive) {
        QJsonObject root;
        root.insert("regionUUID", uuid);
        root.insert("gridLocation", gridLocation);
        webSockClient->sendCommandObject("moveRegion", root);
    }
}

void Session::setRegionTrack(QString regionUUID, QString trackUUID) {
    if (isActive) {
        qDebug() << "Sending region track command";
        QJsonObject root;
        root.insert("trackUUID", trackUUID);
        root.insert("regionUUID", regionUUID);
        webSockClient->sendCommandObject("setRegionTrack", root);
    }
}

void Session::setTrackMute(QString uuid, bool mute) {
    if (isActive) {
        qDebug() << "Sending track mute command";
        QJsonObject root;
        root.insert("trackUUID", uuid);
        root.insert("mute", mute);
        webSockClient->sendCommandObject("setTrackMute", root);
    }
}

void Session::setTrackPan(QString uuid, float pan) {
    if (isActive) {
        qDebug() << "Sending track pan command";
        QJsonObject root;
        root.insert("trackUUID", uuid);
        root.insert("pan", pan);
        webSockClient->sendCommandObject("setTrackPan", root);
    }
}

void Session::setTrackGain(QString uuid, float gain) {
    if (isActive) {
        qDebug() << "Sending track gain command";
        QJsonObject root;
        root.insert("trackUUID", uuid);
        root.insert("gain", gain);
        webSockClient->sendCommandObject("setTrackGain", root);
    }
}

void Session::addRegion(QString uuid, double position, QString file, QString trackUUID) {

}

void Session::disconnectSession() {
    if (getActive() == true) {
        debug::out(3, "Disconnecting Session...");
        webSockClient->sendCommand("disconnect");
    }
}

void Session::closeSession() {
    if (getActive() == true) {
        debug::out(3, "Closing Session...");
        QJsonDocument doc;
        QJsonObject root;
        root.insert("userUUID", userUUID);
        root.insert("sessionUUID", sessionUUID);
        doc.setObject(root);

        QByteArray postDataSize = QByteArray::number(doc.toJson().size());

        netRequest.setRawHeader("Content-Length", postDataSize);
        endPoint = "deleteSession";
        netRequest.setUrl(QUrl("https://horizon-rtc.systems/sessions/deleteSession"));
        netManager->post(netRequest, doc.toJson());
        delete webSockClient;
        isActive = false;
    }
}

void Session::uploadFile(QString fileName, QString hash) {
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;


    fileName = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Horizon" + fileName;
    qDebug() << "FILE NAME" << fileName;
    //filePart.setRawHeader("token", webSockClient->getToken().toUtf8());
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"audioFile\"; filename=\"" + QFileInfo(fileName).fileName() + "\""));
    QFile *file = new QFile(fileName);
    if (file->open(QIODevice::ReadOnly)) {
        filePart.setBodyDevice(file);
        file->setParent(multiPart);
    }

    multiPart->append(filePart);



   QNetworkRequest uploadRequest;
    uploadRequest.setRawHeader("hash", hash.toUtf8());
    uploadRequest.setRawHeader("token", webSockClient->getToken().toUtf8());
    endPoint = "upload";
    uploadRequest.setUrl(QUrl("https://horizon-rtc.systems/sessions/upload/" + sessionUUID));
    statusReply = netManager->post(uploadRequest, multiPart);
    mainWindow->connect(statusReply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(updateSyncProgress(qint64, qint64)));

}

void Session::startUploads() {

    QList<QString> fileNames;
    for (int i = 0; i < uploadQueue.size(); ++i) {
        fileNames.append(uploadQueue.at(i).at(0).split("/")[3]);
    }

    udStatusWindow = new UDStatusWindow(mainWindow, fileNames);
    udStatusWindow->show();

    filesUploaded = 0;
    uploadFile(uploadQueue.at(0).at(0), uploadQueue.at(0).at(1));
}

void Session::downloadFile(QString fileName) {
    qDebug() << "DOWNLOADING FILE";
     QNetworkRequest downloadRequest;

     downloadRequest.setRawHeader("token", webSockClient->getToken().toUtf8());
     endPoint = "download";
     downloadRequest.setUrl(QUrl("https://horizon-rtc.systems/sessions/download/" + fileName.split("/")[1] + "/" + fileName.split("/")[2]));
     statusReply = netManager->get(downloadRequest);
     mainWindow->connect(statusReply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateSyncProgress(qint64, qint64)));

}

void Session::startDownloads() {
    filesDownloaded = 0;
    QList<QString> fileNames;
    for (int i = 0; i < downloadQueue.size(); i++) {
        fileNames.append(downloadQueue.at(i).split("/")[3]);
    }
    udStatusWindow = new UDStatusWindow(mainWindow, fileNames);
    udStatusWindow->show();
    downloadFile(downloadQueue.at(0));
}

void Session::updateSyncProgress(qint64 current, qint64 total) {
    if (udStatusWindow != nullptr) {
        udStatusWindow->updateMax(total);
        udStatusWindow->updateProgress(current);
    }
}

