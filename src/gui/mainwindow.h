#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>


#include "network/uac.h"

#include "arrangewidget.h"
#include "mixerwidget.h"
#include "preferenceswindow.h"
#include "librarywidget.h"
#include "infowidget.h"
#include "rtchostwindow.h"
#include "rtcclientwindow.h"
#include "renderwindow.h"
#include "colorpickerwidget.h"
#include "guiupdatethread.h"
#include "debugprofilerwindow.h"

#include "app/audiomanager.h"
#include "common/thememanager.h"
#include "common/dialogs.h"
#include "app/projectserialization.h"


#include "splashscreen.h"
#include "common/preferences.h"

#include <QTimer>
#include <QWidget>
#include <QUrl>
#include <QLayout>
#include <QApplication>
#include <QFileOpenEvent>



//class AudioManager;

class RTCClientWindow;
class RTCHostWindow;
class ProjectSerialization;
//class AudioManager;
class ArrangeWidget;
class MixerWidget;
class LibraryWidget;
class InfoWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, SplashScreen *splashScreen = nullptr, Preferences *prefs = nullptr);
    ~MainWindow();

    std::unique_ptr<AudioManager> audioMan;
    ThemeManager *themeMan;
    Timeline *timeline;
    Preferences *prefs;


    QTimer *uiTimer;

    GuiUpdateThread *updateThread;

    InfoWidget *infoWidget;
    ArrangeWidget *arrangeWidget;
    MixerWidget *mixerWidget;
    LibraryWidget *libraryWidget;



    void updateIconThemes();
    void togglePlayback();


    void openProject(QString fileName);
    void saveProject(QString fileName = "Untitled");
    void loadProjectJSON(QString json);
    void newProject();

    bool isProjectEdited();
    bool ensureSaved();

    void newTrack(QColor color, QString uuid);

private slots:


    void on_playButton_clicked();

    void on_actionAbout_triggered();

    void on_stopButton_clicked();

    void on_actionPreferences_triggered();

    void on_importAudioFileMenu_triggered();

    void on_newAudioTrackMenu_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionNew_Project_triggered();

    void on_actionOpen_triggered();

    void closeEvent(QCloseEvent *e) override;

    void on_actionArrange_toggled(bool arg1);

    void on_actionMixer_toggled(bool arg1);

    void on_actionConnect_to_Session_2_triggered();

    void on_actionManage_Live_Session_triggered();

    void on_liveButton_clicked();

    void on_actionRender_Audio_triggered();

    void on_pushButton_3_clicked();

    void on_tempo_lcd_valueChanged(double arg1);

    void on_actionLibrary_toggled(bool arg1);

    void on_actionColor_Picker_toggled(bool arg1);

    void on_actionPropery_Editor_toggled(bool arg1);

    void on_actionProfiler_triggered();

protected:

private:
    Ui::MainWindow *ui;

    void uiUpdate();

    QString loadFile(QString path);
    QString loadedProjectPath;

    QString untitledJSON;

    RTCHostWindow *rtcHost;
    RTCClientWindow *rtcClient;

    ProjectSerialization *serialization;

    Session *session;
    UAC *uac;

    ColorPickerWidget *colorPicker;
public slots:
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
};
#endif // MAINWINDOW_H
