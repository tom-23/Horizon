#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <sstream>

#include <QWidget>

#ifndef _WIN32
    #include "CoreFoundation/CoreFoundation.h"
    #include "darwin/haptictouchpad.h"
    #include "darwin/touchbar.h"
#else
    #include <Windows.h>
#endif

namespace util {

    #ifndef _WIN32
        std::string getResourceBundle();
        void macTouchpadVibrateSmall();
        void macTouchpadVibrateHard();

        void macInitTouchbar(QWidget *parent);
    #else
        std::string getInstallDir();
    #endif


}
#endif
