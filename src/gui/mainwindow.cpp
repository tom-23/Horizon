#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    QString themeLoc;

    ui->footer->hide();

    loadedProject = "Untitled";

    #ifndef _WIN32
        themeLoc = QString::fromUtf8(util::getResourceBundle().c_str()) + "/themes/sand-castle.json";
    #else
        themeLoc = QString::fromUtf8(util::getInstallDir().c_str()) + "/themes/default-dark.json";
    #endif

    themeMan = new ThemeManager(this, themeLoc);
    dialogs::setParent(this);
    dialogs::setThemeManager(themeMan);


    updateIconThemes();

    debug::out(3, "Initalising Session...");
    session = new Session();



    debug::out(3, "Loading audio manager...");


    arrangeWidget = new ArrangeWidget(this);
    audioMan = std::make_unique<AudioManager>(*arrangeWidget->tl);

    arrangeWidget->setAudioManager(*audioMan);

    debug::out(3, "Setting BPM");
    audioMan->setBPM(150.0);
    audioMan->setDivision(4);
    audioMan->setLookAhead(0.05);

    debug::out(3, "Loaded audio manager with default settings!");

    
    ui->content->layout()->addWidget(arrangeWidget);
    arrangeWidget->show();
    arrangeWidget->tl->setColorTheme(themeMan);
    InfoWidget* iw;
    iw = new InfoWidget(this);
    ui->content->layout()->addWidget(iw);
    iw->show();

    LibraryWidget* lb;
    lb = new LibraryWidget(this);
    ui->content->layout()->addWidget(lb);
    lb->show();

    EffectWidget* ew;
    ew = new EffectWidget(iw);
    iw->addEffect(ew);

    uiTimer = new QTimer(parent);
    connect(uiTimer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::uiUpdate));


    newProject();
}

void MainWindow::uiUpdate() {
    arrangeWidget->tl->setPlayheadLocation(audioMan->getCurrentGridTime());

    float glr = float(floor(audioMan->getCurrentGridTime() * arrangeWidget->tl->barLength)) / arrangeWidget->tl->barLength;
    ui->barNumberLabel->setText(QString::number(floor(audioMan->getCurrentGridTime())));
    ui->beatNumberLabel->setText(QString::number(((glr - floor(audioMan->getCurrentGridTime())) * arrangeWidget->tl->barLength) + 1));
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_playButton_clicked()
{
    togglePlayback();

}

void MainWindow::on_actionAbout_triggered()
{
    new dialogs::AboutDialog();
}


void MainWindow::on_stopButton_clicked()
{
    audioMan->stop();
    uiTimer->stop();
    arrangeWidget->tl->setPlayheadLocation(0.0);
}

void MainWindow::updateIconThemes() {
    ui->playButton->setStyleSheet("image: url('" + themeMan->colorizeSVG(":/svg/svg/play.svg") + "');");
    ui->stopButton->setStyleSheet("image: url('" + themeMan->colorizeSVG(":/svg/svg/stop.svg") + "');");
    ui->recordButton->setStyleSheet("image: url('" + themeMan->colorizeSVG(":/svg/svg/record.svg") + "');");
}



void MainWindow::on_actionPreferences_triggered()
{
    PreferencesWindow *pw = new PreferencesWindow(this);
    pw->show();
}


void MainWindow::togglePlayback() {
    if (audioMan->isPlaying == false) {
        uiTimer->start(30);
        audioMan->play();
    } else {
        uiTimer->stop();
        audioMan->pause();
    }
}

void MainWindow::keyPressEvent(QKeyEvent* ke) {
    switch (ke->key()) {

        case Qt::Key::Key_Space:
            togglePlayback();
        break;

        case Qt::Key::Key_Alt:
            qDebug() << "REGION SNAPPING DISABLED";
            arrangeWidget->tl->regionSnapping = false;
        break;

    }
    QMainWindow::keyPressEvent(ke); // base class implementation
}

void MainWindow::keyReleaseEvent(QKeyEvent* ke) {
    switch (ke->key()) {
        case Qt::Key::Key_Alt:
            arrangeWidget->tl->regionSnapping = true;
        break;

    }
    QMainWindow::keyReleaseEvent(ke); // base class implementation
}



void MainWindow::on_importAudioFileMenu_triggered()
{
    arrangeWidget->importAudio();
}


void MainWindow::on_newAudioTrackMenu_triggered()
{
    arrangeWidget->addNewAudioTrack();
}

void MainWindow::on_connectButton_clicked()
{
    rtcHost = new RTCHostWindow(this, session);
    rtcHost->show();


}

void MainWindow::on_actionSave_triggered()
{
    saveProject(QString::fromStdString(loadedProject));

}

void MainWindow::saveProject(QString fileName) {
    if (fileName != "Untitled") {
        QFile outputFile(fileName);


        if (outputFile.open(QIODevice::ReadWrite) )
        {
            ProjectSerialization *serialization = new ProjectSerialization;

            outputFile.write(QString::fromStdString(serialization->serialize(*audioMan)).toUtf8());
            loadedProject = QString(fileName).toStdString();


        } else {
            dialogs::MessageDialog::show("Cannot save file", "An error occoured whilst trying to load this file. This could be due to permission issues.",
                                         dialogs::MessageDialog::icons::no,
                                         dialogs::MessageDialog::buttons::okOnly);
        }

    } else {
        QString dialogFileName = QFileDialog::getSaveFileName(this,
                tr("Save Project"), "",
                tr("Horizon Project File (*.hzp)"));
        if (dialogFileName != "") {
            saveProject(dialogFileName);
        }

    }


}

void MainWindow::newProject() {
    if (isProjectEdited() == false) {
        loadedProject = "Untitled";
        this->setWindowTitle("Untitled");
        //arrangeWidget = new ArrangeWidget(this);
        //audioMan = std::make_unique<AudioManager>(*arrangeWidget->tl);
    }

}
void MainWindow::ensureSaved() {

}

bool MainWindow::isProjectEdited() {
    if (loadedProject != "Untitled") {
        QFile loadedProjectFile(QString::fromStdString(loadedProject));
        if(loadedProjectFile.open(QIODevice::ReadOnly)) {

        } else {
            dialogs::MessageDialog::show("Cannot open loaded project", "An error occoured whilst trying to load this file. This could be due to permission issues.",
                                         dialogs::MessageDialog::icons::no,
                                         dialogs::MessageDialog::buttons::okOnly);
        }
    } else {
        return false;
    }
}
