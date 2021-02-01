#include "guiupdatethread.h"

GuiUpdateThread::GuiUpdateThread(QObject *parent, std::function<void ()> _updateFunction) : QThread(parent)
{
    updateFunction = _updateFunction;
    this->setTerminationEnabled(false);
}

void GuiUpdateThread::run() {
    uiUpdateTimer = new QTimer(this);
    connect(uiUpdateTimer, &QTimer::timeout, this, &GuiUpdateThread::uiTimeOut  );
    uiUpdateTimer->start(10);
}

void GuiUpdateThread::uiTimeOut() {
    updateFunction();
}
