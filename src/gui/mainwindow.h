#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "infowidget.h"
#include "arrangewidget.h"
#include "preferenceswindow.h"
#include "librarywidget.h"
#include "aboutdialog.h"
#include "app/audiomanager.h"
#include "common/thememanager.h"
#include <QTimer>

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

private slots:


    void on_playButton_clicked();

    void on_actionAbout_triggered();

    void on_stopButton_clicked();

    void on_actionPreferences_triggered();

private:
    Ui::MainWindow *ui;

    void uiUpdate();

protected:

};
#endif // MAINWINDOW_H
