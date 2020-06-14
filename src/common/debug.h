#ifndef DEBUG_H
#define DEBUG_H


#include <iostream>

namespace debug {
    static int overallDebugLevel;
    void setDebugLevel(int level);
    int getDebugLevel();
    void out(int level, std::string message);
}





#endif // DEBUG_H
