#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "infowidget.h"
#include "arrangewidget.h"
#include "preferenceswindow.h"
#include "librarywidget.h"
#include "rtchostwindow.h"

#include "app/audiomanager.h"
#include "common/thememanager.h"
#include "common/dialogs.h"
#include "app/projectserialization.h"

#include "network/session.h"

#include <QTimer>
#include <QWidget>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::unique_ptr<AudioManager> audioMan;
    ThemeManager *themeMan;
    Timeline *timeline;


    QTimer *uiTimer;

    ArrangeWidget *arrangeWidget;

    void updateIconThemes();
    void togglePlayback();

    std::string loadedProject;

    void saveProject(QString fileName = "Untitled");
    void newProject();

    bool isProjectEdited();
    void ensureSaved();



private slots:


    void on_playButton_clicked();

    void on_actionAbout_triggered();

    void on_stopButton_clicked();

    void on_actionPreferences_triggered();

    void on_importAudioFileMenu_triggered();

    void on_newAudioTrackMenu_triggered();

    void on_connectButton_clicked();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;

    void uiUpdate();

    RTCHostWindow *rtcHost;

    Session *session;


public slots:
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);

};
#endif // MAINWINDOW_H
