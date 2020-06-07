QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += \
    c++14 \
    app_bundle


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/aboutdialog.cpp \
    src/arrangewidget.cpp \
    src/audiomanager.cpp \
    src/audioregionmanager.cpp \
    src/colorpickerwidget.cpp \
    src/debug.cpp \
    src/effectwidget.cpp \
    src/ghostplayhead.cpp \
    src/graphicsscene.cpp \
    src/graphicsview.cpp \
    src/grid.cpp \
    src/infowidget.cpp \
    src/librarywidget.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/playhead.cpp \
    src/region.cpp \
    src/regiongraphicitem.cpp \
    src/rulergraphicwidget.cpp \
    src/thememanager.cpp \
    src/timeline.cpp \
    src/timelinegraphicwidget.cpp \
    src/timer.cpp \
    src/track.cpp \
    src/trackaudio.cpp \
    src/trackcontrolswidget.cpp

HEADERS += \
    src/aboutdialog.h \
    src/arrangewidget.h \
    src/audiomanager.h \
    src/audioregionmanager.h \
    src/audioutil.h \
    src/colorpickerwidget.h \
    src/debug.h \
    src/effectwidget.h \
    src/ghostplayhead.h \
    src/graphicsscene.h \
    src/graphicsview.h \
    src/grid.h \
    src/infowidget.h \
    src/librarywidget.h \
    src/mainwindow.h \
    src/playhead.h \
    src/region.h \
    src/regiongraphicitem.h \
    src/rulergraphicwidget.h \
    src/thememanager.h \
    src/timeline.h \
    src/timelinegraphicwidget.h \
    src/timer.h \
    src/track.h \
    src/trackaudio.h \
    src/trackcontrolswidget.h

FORMS += \
    src/aboutdialog.ui \
    src/arrangewidget.ui \
    src/colorpickerwidget.ui \
    src/effectwidget.ui \
    src/infowidget.ui \
    src/librarywidget.ui \
    src/mainwindow.ui \
    src/trackcontrolswidget.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets/core.qrc \
    assets/resources.qrc \
    assets/themes.qrc

install_it.path = %{buildDir}
install_it.files += %{sourceDir}/Core
install_it.files += %{sourceDir}/themes

INSTALLS += install_it

INCLUDEPATH += $$PWD/lib/LabSound/include
DEPENDPATH += $$PWD/lib/LabSound/include

mac: LIBS += -F$$PWD/lib/LabSound/build/bin/ -framework LabSound

macx: LIBS += -L$$PWD/lib/LabSound/build/bin/ -llibnyquist
macx: PRE_TARGETDEPS += $$PWD/lib/LabSound/build/bin/liblibnyquist.a

macx: LIBS += -L$$PWD/lib/LabSound/build/bin/ -llibwavpack
macx: PRE_TARGETDEPS += $$PWD/lib/LabSound/build/bin/liblibwavpack.a

mac: LIBS += -framework Cocoa
mac: LIBS += -framework Accelerate
mac: LIBS += -framework CoreAudio
mac: LIBS += -framework AudioUnit
mac: LIBS += -framework AudioToolbox


ICON = assets/app_icon.icns

coreContentFiles.files = $$PWD/assets/core
coreContentFiles.path = Contents/Resources
QMAKE_BUNDLE_DATA += coreContentFiles

themeContentFiles.files = $$PWD/assets/themes
themeContentFiles.path = Contents/Resources
QMAKE_BUNDLE_DATA += themeContentFiles

win32: LIBS += -ldsound

win32: LIBS += -ldxguid

win32: LIBS += -lWinMM

win32: LIBS += -L$$PWD/lib/LabSound/build/bin/ -lLabSound
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/LabSound/build/bin/LabSound.lib

win32: LIBS += -L$$PWD/lib/LabSound/build/bin/ -llibnyquist
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/LabSound/build/bin/libnyquist.lib

win32: LIBS += -L$$PWD/lib/LabSound/build/bin/ -llibwavpack
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/LabSound/build/bin/libwavpack.lib
