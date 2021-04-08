#include <QApplication>
#include <QFileOpenEvent>
#include <QtDebug>

#include "gui/splashscreen.h"
#include "gui/mainwindow.h"
#include "common/debug.h"
#include "common/util.h"
#include "common/preferences.h"
#include "common/version.h"

class Horizon : public QApplication
{
public:

    Horizon(int &argc, char **argv) : QApplication(argc, argv) {

        this->instance();
        this->setAttribute(Qt::AA_UseHighDpiPixmaps); // Enable HiDPi Image rendering

        debug::setDebugLevel(3); // Setting the debug level. On production builds, this probably be set to 1
        debug::out(3, "Horizon Digital Audio Workstation");

        debug::out(3, "Loading fonts...");

        // Add the custom fonts
        QFontDatabase::addApplicationFont(":/fonts/fonts/Rublik/Rubik-Regular.ttf");
        QFontDatabase::addApplicationFont(":/fonts/fonts/Rublik/Rubik-Medium.ttf");
        QFontDatabase::addApplicationFont(":/fonts/fonts/Rublik/Rubik-MediumItalic.ttf");
        QFontDatabase::addApplicationFont(":/fonts/fonts/Rublik/Rubik-Bold.ttf");
        QFontDatabase::addApplicationFont(":/fonts/fonts/Rublik/Rubik-Italic.ttf");

        debug::out(3, "Showing Splash...");

        // Show the splash screen
        SplashScreen *splashScreen = new SplashScreen();
        splashScreen->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        splashScreen->show();

        splashScreen->setVersion(HORIZON_VERSION);

        splashScreen->setText("Loading Preferences...");

        // Grab the preferences location. On windows, use the install directory and on macOS, we use the app bundle.
        QString prefsLoc;

        #ifndef _WIN32
        prefsLoc = QString::fromStdString(util::getResourceBundle()) + "/prefs.json";
        #else
        prefsLoc = QString::fromStdString(util::getInstallDir()) + "/prefs.json";
        #endif

        Preferences *prefs = new Preferences(prefsLoc);
        prefs->load();

        debug::out(3, "Loading MainWindow...");
        splashScreen->setText("Loading main window...");

        // init the main window
        mainWindow = new MainWindow(nullptr, splashScreen, prefs);
        mainWindow->show();

        this->instance()->exec();
    }

    MainWindow *mainWindow;


    // this event handles the user double clicking a project file to open it. This is only working on macOS.
    bool event(QEvent *event) override
    {
        if (event->type() == QEvent::FileOpen) {
            QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
            debug::out(3, "Recieved input file. Opening...");
            mainWindow->openProject(openEvent->file());
        }

        return QApplication::event(event);
    }
};
