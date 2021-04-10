#include "enginethread.h"

EngineThread::EngineThread(QObject *parent) : QThread(parent)
{
    engine = new QProcess(this);
    engine->setProcessChannelMode(QProcess::ForwardedChannels);
    engine->setInputChannelMode(QProcess::InputChannelMode::ForwardedInputChannel);
    engine->setProgram("HorizonEngine");

}

void EngineThread::run() {
    debug::out(3, "Starting engine, waiting for listening message...");
    engine->start(QIODevice::ReadOnly);
    engine->waitForStarted();
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
