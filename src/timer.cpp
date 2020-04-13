#include "timer.h"

Timer::Timer()
{

}

template<typename Function>
void Timer::setTimeout(Function function, int delay) {
    this->clear = false;
    std::thread t([=]() {
        if(this->clear) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if(this->clear) return;
        function();
    });
    t.detach();
}

template<typename Function>
void Timer::setInterval(Function function, int intervalMicSec) {
    this->clear = false;
    std::thread t([=]() {
        while(true) {
            if(this->clear) return;
            std::this_thread::sleep_for(std::chrono::microseconds(intervalMicSec));
            if(this->clear) return;
            function();
        }
    });
    t.detach();
}
