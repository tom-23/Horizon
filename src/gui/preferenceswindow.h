#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QDialog>

namespace Ui {
class PreferencesWindow;
}

class PreferencesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesWindow(QWidget *parent = nullptr);
    ~PreferencesWindow();

private:
    Ui::PreferencesWindow *ui;
};

#endif // PREFERENCESWINDOW_H
