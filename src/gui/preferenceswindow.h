#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QDialog>
#include <QDir>
#include "common/util.h"
#include "common/preferences.h"
#include <QFileDialog>

namespace Ui {
class PreferencesWindow;
}

class PreferencesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesWindow(QWidget *parent = nullptr, Preferences *prefs = nullptr);
    ~PreferencesWindow();


private slots:
    void on_showMixerButton_toggled(bool checked);

    void on_cancelButton_clicked();

    void on_applyButton_clicked();

    void on_okButton_clicked();

    void on_showLibraryButton_toggled(bool checked);

    void on_addFolderButton_clicked();

    void on_removeFolderButton_clicked();

private:
    Ui::PreferencesWindow *ui;
    QString bundleLoc;
    Preferences *prefs;
};

#endif // PREFERENCESWINDOW_H
