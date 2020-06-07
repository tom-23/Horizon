#include "debug.h"
#include <QDebug>

namespace debug {
    void setDebugLevel(int level) {
        overallDebugLevel = level;
    }

    int getDebugLevel() {
        return overallDebugLevel;
    }

    void out(int level, std::string message) {
        if (level == 1 && overallDebugLevel >= 1) {
            std::cout << "[ERROR] " << message << std::endl;
        } else if (level == 2 && overallDebugLevel >= 2) {
            std::cout << "[WARNING] " << message << std::endl;
        } else if (level == 3 && overallDebugLevel >= 3) {
            std::cout << "[INFO] " << message << std::endl;
        }

    }

}
