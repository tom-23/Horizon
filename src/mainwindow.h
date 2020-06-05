#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "arrangewidget.h"
#include "infowidget.h"
#include "librarywidget.h"
#include "audiomanager.h"
#include "thememanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    AudioManager *audioMan;
    ThemeManager *themeMan;

    ArrangeWidget* ar;


private slots:


    void on_playButton_clicked();

    void on_pushButton_clicked();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

protected:

};
#endif // MAINWINDOW_H
