#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <sstream>

#ifndef _WIN32
    #include "CoreFoundation/CoreFoundation.h"
#else
    #include <Windows.h>
#endif

namespace util {

    #ifndef _WIN32
        std::string getResourceBundle();
    #else
        std::string getInstallDir();
    #endif

}
#endif
