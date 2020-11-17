#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include "app/audiomanager.h"


namespace Ui {
class RenderWindow;
}

class RenderWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RenderWindow(QWidget *parent = nullptr, AudioManager *audioMan = nullptr);
    ~RenderWindow();

private slots:
    void on_cancel_clicked();

    void on_startButton_clicked();

private:
    Ui::RenderWindow *ui;
    AudioManager *audioMan;
};

#endif // RENDERWINDOW_H
