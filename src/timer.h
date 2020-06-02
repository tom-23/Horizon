#ifndef TIMER_H
#define TIMER_H
#include <chrono>
#include <iostream>
#include <thread>

class Timer
{
    bool clear = false;
public:
    Timer();

    template<typename Function>
    void setTimeout(Function function, int delay);

    template<typename Function>
    void setInterval(Function function, int interval);

    void stop();

private:


};

#endif // TIMER_H
