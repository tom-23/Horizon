#include "util.h"

namespace util {
    //#ifndef _WIN32
    std::string getResourceBundle() {
        
            CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
            CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef, kCFURLPOSIXPathStyle);
            std::string path(CFStringGetCStringPtr(macPath, CFStringGetSystemEncoding()));
            CFRelease(appUrlRef);
            CFRelease(macPath);
            return path.append("/Contents/Resources");
        
    }
    //#endif
}