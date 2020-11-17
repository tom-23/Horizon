#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <sstream>

#ifndef _WIN32
    #include "CoreFoundation/CoreFoundation.h"
    #include "darwin/haptictouchpad.h"
#else
    #include <Windows.h>
#endif

namespace util {

    #ifndef _WIN32
        std::string getResourceBundle();
        void macTouchpadVibrateSmall();
        void macTouchpadVibrateHard();
    #else
        std::string getInstallDir();
    #endif

}
#endif
