#include "enginethread.h"

EngineThread::EngineThread()
{
    engine = new QProcess(this);
    engine->setProcessChannelMode(QProcess::ForwardedChannels);
    engine->setInputChannelMode(QProcess::ForwardedInputChannel);
    engine->setProgram("HorizonEngine");
}

void EngineThread::run() {

    //engine->connect(engine, &EngineThread::finished, this, &EngineThread::exited);
    engine->start();
}

bool EngineThread::isRunning() {
    return (engine->state() != QProcess::NotRunning);
}

void EngineThread::kill() {
    engine->kill();
}

void EngineThread::exited(int exitCode) {
    qDebug() << "HorizonEngine Exited" << exitCode;
}
