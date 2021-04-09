#ifndef ENGINETHREAD_H
#define ENGINETHREAD_H

#include <QThread>
#include <QObject>
#include <QProcess>
#include <iostream>
#include <QDebug>

#include "common/debug.h"

class EngineThread : public QThread
{
public:
    EngineThread();

    void run() override;
    void kill();
    bool isRunning();
private:
    QProcess *engine;
signals:
    void on_engine_closed();
private slots:
    void exited(int exitCode);
};

#endif // ENGINETHREAD_H
