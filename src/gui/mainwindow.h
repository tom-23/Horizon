#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>


#include "network/uac.h"


#include "infowidget.h"
#include "arrangewidget.h"
#include "mixerwidget.h"
#include "preferenceswindow.h"
#include "librarywidget.h"
#include "rtchostwindow.h"
#include "rtcclientwindow.h"

#include "app/audiomanager.h"
#include "common/thememanager.h"
#include "common/dialogs.h"
#include "app/projectserialization.h"


#include "splashscreen.h"

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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, SplashScreen *splashScreen = nullptr);
    ~MainWindow();

    std::unique_ptr<AudioManager> audioMan;
    ThemeManager *themeMan;
    Timeline *timeline;


    QTimer *uiTimer;

    ArrangeWidget *arrangeWidget;
    MixerWidget *mixerWidget;


    void updateIconThemes();
    void togglePlayback();


    void openProject(QString fileName);
    void saveProject(QString fileName = "Untitled");
    void loadProjectJSON(QString json);
    void newProject();

    bool isProjectEdited();
    bool ensureSaved();

    void newTrack(QColor color, QString uuid);
    void moveRegion(QString uuid, double gridLocation);
    void selectTrack(QString uuid);


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

public slots:
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
};
#endif // MAINWINDOW_H
