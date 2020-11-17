#include "timerex.h"

TimerEX::TimerEX(QObject *parent, std::function<void()> timeout) : QObject(parent)
{
    timer = new QTimer(parent);
    connect(timer, &QTimer::timeout, this, timeout);
}

void TimerEX::start() {
    timer->start(30);
}

void TimerEX::stop() {
    timer->stop();
}
