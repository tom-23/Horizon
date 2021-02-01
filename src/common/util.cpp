#include "util.h"

namespace util {
    #ifndef _WIN32
    std::string getResourceBundle() {
        
            CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
            CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef, kCFURLPOSIXPathStyle);
            std::string path(CFStringGetCStringPtr(macPath, CFStringGetSystemEncoding()));
            CFRelease(appUrlRef);
            CFRelease(macPath);
            return path.append("/Contents/Resources");
        
    }

    void macTouchpadVibrateSmall() {
        vibrateSmall();
    }

    void macTouchpadVibrateHard() {
        vibrateHard();
    }

    void macInitTouchbar(QWidget *parent) {
        initTouchbar(parent);
    }

    #else
    std::string getInstallDir() {
        char buffer[MAX_PATH];
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::string::size_type pos = std::string(buffer).find_last_of("\\/");
        std::string bsPath = std::string(buffer).substr(0, pos);
        return bsPath;
    }

    #endif




}
