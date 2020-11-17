#ifndef TIMEREX_H
#define TIMEREX_H

#include <QObject>
#include <QTimer>

class TimerEX : public QObject
{
    Q_OBJECT
public:
    explicit TimerEX(QObject *parent = nullptr, std::function<void()> timeout = nullptr);
    void start();
    void stop();
signals:

private:
    QTimer *timer;
};

#endif // TIMEREX_H
