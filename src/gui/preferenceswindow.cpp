#include "preferenceswindow.h"
#include "ui_preferenceswindow.h"

PreferencesWindow::PreferencesWindow(QWidget *parent, Preferences *_prefs) :
    QDialog(parent),
    ui(new Ui::PreferencesWindow)
{
    ui->setupUi(this);

    prefs = _prefs;

    #ifndef _WIN32
    bundleLoc = QString::fromUtf8(util::getResourceBundle().c_str());
    #else
    bundleLoc = QString::fromUtf8(util::getInstallDir().c_str());
    #endif

    QDir themeDir(bundleLoc + "/themes");
    QStringList themes = themeDir.entryList(QStringList() << "*.json",QDir::Files);
    foreach(QString filename, themes) {
        ui->themeBox->addItem(filename.split(".")[0]);
    }
    ui->themeBox->setCurrentText(prefs->themeName);
    ui->langBox->setCurrentText(prefs->language);
    ui->showMixerButton->setChecked(prefs->showMixer);
    ui->showLibraryButton->setChecked(prefs->showLibrary);

    ui->foldersList->addItems(prefs->libraryDirList);
}


PreferencesWindow::~PreferencesWindow()
{
    delete ui;
}

void PreferencesWindow::on_showMixerButton_toggled(bool checked)
{
    if (checked) {
        ui->showMixerButton->setText("On");
    } else {
        ui->showMixerButton->setText("Off");
    }
}

void PreferencesWindow::on_cancelButton_clicked()
{
    this->close();
}

void PreferencesWindow::on_applyButton_clicked()
{
    prefs->showMixer = ui->showMixerButton->isChecked();
    prefs->showLibrary = ui->showLibraryButton->isChecked();
    prefs->themeName = ui->themeBox->currentText();
    prefs->language = ui->langBox->currentText();
    prefs->libraryDirList.clear();
    for (int i = 0; i < ui->foldersList->count(); ++i) {
        prefs->libraryDirList.append(ui->foldersList->item(i)->text());
    }

    prefs->save();
}

void PreferencesWindow::on_okButton_clicked()
{
    ui->applyButton->click();
    this->close();
}

void PreferencesWindow::on_showLibraryButton_toggled(bool checked)
{
    if (checked) {
        ui->showLibraryButton->setText("On");
    } else {
        ui->showLibraryButton->setText("Off");
    }
}

void PreferencesWindow::on_addFolderButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Folder..."),
                                                    "/",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (dir != "") {
        ui->foldersList->addItem(dir);
    }
}

void PreferencesWindow::on_removeFolderButton_clicked()
{
    QList<QListWidgetItem*> items = ui->foldersList->selectedItems();

    foreach(QListWidgetItem* item, items){
        ui->foldersList->removeItemWidget(item);
        delete item;
    }
}
