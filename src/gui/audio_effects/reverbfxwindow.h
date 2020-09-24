#ifndef REVERBFXWINDOW_H
#define REVERBFXWINDOW_H

#include <QDialog>

namespace Ui {
class ReverbFXWindow;
}

class ReverbFXWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReverbFXWindow(QWidget *parent = nullptr);
    ~ReverbFXWindow();

private:
    Ui::ReverbFXWindow *ui;
};

#endif // REVERBFXWINDOW_H
