#ifndef ENGINETHREAD_H
#define ENGINETHREAD_H

#include <QThread>
#include <QObject>
#include <QProcess>
#include <iostream>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>

#include "common/debug.h"

class EngineThread : public QThread
{
    Q_OBJECT
public:
    EngineThread(QObject *parent = nullptr);

    void run() override;
    void kill();
    bool isRunning();
    bool isListening = false;
private:
    QProcess *engine;
signals:
    void on_engine_closed();
    void on_isListening();
private slots:
    void exited(int exitCode);
};

#endif // ENGINETHREAD_H
