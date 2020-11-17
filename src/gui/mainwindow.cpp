#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, SplashScreen *splashScreen)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    QString themeLoc;

    ui->footer->hide();

    loadedProjectPath = "Untitled";

    #ifndef _WIN32
        themeLoc = QString::fromUtf8(util::getResourceBundle().c_str()) + "/themes/default-dark.json";
    #else
        themeLoc = QString::fromUtf8(util::getInstallDir().c_str()) + "/themes/default-dark.json";
    #endif

    themeMan = new ThemeManager(this, themeLoc);
    dialogs::setParent(this);
    dialogs::setThemeManager(themeMan);


    updateIconThemes();

    splashScreen->setText("Loading audio engine...");
    debug::out(3, "Loading audio manager...");


    arrangeWidget = new ArrangeWidget(this);
    audioMan = std::make_unique<AudioManager>(this, *arrangeWidget->tl);
    session = audioMan->session;
    arrangeWidget->setAudioManager(*audioMan);

    mixerWidget = new MixerWidget(this);
    arrangeWidget->setMixer(mixerWidget->mixer);

    splashScreen->setText("Initialising UAC...");
    debug::out(3, "Initialising UAC...");
    uac = new UAC();

    debug::out(3, "Setting BPM");
    audioMan->setBPM(130.0);
    audioMan->setDivision(4);
    audioMan->setLookAhead(0.05);

    debug::out(3, "Loaded audio manager with default settings!");


    ui->contentVertical->addWidget(arrangeWidget);
    ui->contentVertical->addWidget(mixerWidget);

    arrangeWidget->show();
    arrangeWidget->tl->setColorTheme(themeMan);

    mixerWidget->show();

    LibraryWidget* lb;
    lb = new LibraryWidget(this);
    ui->content->layout()->addWidget(lb);
    lb->show();

    //EffectWidget* ew;
   // ew = new EffectWidget(iw);
    //iw->addEffect(ew);

    uiTimer = new QTimer(this);
    connect(uiTimer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::uiUpdate));


    splashScreen->setText("Creating new project...");
    serialization = new ProjectSerialization();

    newProject();

    splashScreen->close();

    ui->tempo_lcd->setAttribute(Qt::WA_MacShowFocusRect, 0);

    //this->show();
    this->showMaximized();

    uiTimer->start(60);


}

void MainWindow::uiUpdate() {
    arrangeWidget->tl->setPlayheadLocation(audioMan->getCurrentGridTime());
    arrangeWidget->tl->updateViewports();

    float glr = float(floor(audioMan->getCurrentGridTime() * arrangeWidget->tl->barLength)) / arrangeWidget->tl->barLength;
    ui->barNumberLabel->setText(QString::number(floor(audioMan->getCurrentGridTime())));
    ui->beatNumberLabel->setText(QString::number(((glr - floor(audioMan->getCurrentGridTime())) * arrangeWidget->tl->barLength) + 1));

    for (int i = 0; i < int(audioMan->getTrackListCount()); i++) {
        audioMan->getTrackByIndex(i)->uiUpdate();
    }
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
    //uiTimer->stop();
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
        //uiTimer->start(30);
        audioMan->play();
    } else {
        //uiTimer->stop();
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
    QColor color = QColor::fromRgb(QRandomGenerator::global()->generate());
    QString uuid = QUuid::createUuid().toString();
    newTrack(color, uuid);
    session->newTrack(color.name(QColor::HexRgb), uuid);
}

void MainWindow::newTrack(QColor color, QString uuid) {
    arrangeWidget->addAudioTrack(nullptr, uuid.toStdString())->updateColor(color);
}

void MainWindow::on_actionNew_Project_triggered()
{
    if (ensureSaved() == true) {
        newProject();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if (ensureSaved() == true) {
        QString dialogFileName = QFileDialog::getOpenFileName(this,
                tr("Open Project"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                tr("Horizon Project File (*.hzp)"));
        if (dialogFileName != "") {
            openProject(dialogFileName);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    saveProject(QString::fromStdString(loadedProjectPath.toStdString()));
}

void MainWindow::on_actionSave_As_triggered()
{
    saveProject();
}

void MainWindow::openProject(QString fileName) {
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly)) {
        audioMan->clearAll();
        arrangeWidget->tl->clearAll();
        loadProjectJSON(inputFile.readAll());
        QFileInfo fileInfo(fileName);
        this->setWindowTitle(fileInfo.fileName());
        loadedProjectPath = fileName;

    } else {

        debug::out(3, "Cannot open project file. It is probably open in another program.");
    }
}

void MainWindow::saveProject(QString fileName) {
    if (fileName != "Untitled") {

        QFile outputFile(fileName);

        if (outputFile.open(QIODevice::ReadWrite) )
        {

            ProjectSerialization *serialization = new ProjectSerialization;

            outputFile.resize(0);
            outputFile.write(QString::fromStdString(serialization->serialize(*audioMan, false)).toUtf8());

            outputFile.flush();
            outputFile.close();

            QFileInfo fileInfo(fileName);
            this->setWindowTitle(fileInfo.fileName());
            loadedProjectPath = fileName;


        } else {
            dialogs::MessageDialog::show("Cannot save file", "An error occoured whilst trying to load this file. This could be due to permission issues.",
                                         dialogs::MessageDialog::icons::no,
                                         dialogs::MessageDialog::buttons::okOnly);
        }

    } else {
        QString dialogFileName = QFileDialog::getSaveFileName(this,
                tr("Save Project"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                tr("Horizon Project File (*.hzp)"));
        if (dialogFileName != "") {
            saveProject(dialogFileName);
        }

    }


}

void MainWindow::newProject() {
        loadedProjectPath = "Untitled";
        this->setWindowTitle(loadedProjectPath);
        audioMan->clearAll();
        arrangeWidget->tl->clearAll();
        untitledJSON = QString::fromStdString(serialization->serialize(*audioMan, false));
}

void MainWindow::loadProjectJSON(QString JSON) {

    //arrangeWidget->setHZoomFactor(50);
    ui->tempo_lcd->setValue(audioMan->getBPM());
    int regionsToBeLoaded = 0;
    debug::out(3, "Loading project JSON...");
    serialization->deSerialize(JSON.toStdString(), *audioMan);
    for (int i = 0; i < audioMan->getTrackListCount(); i++) {
        arrangeWidget->addAudioTrack(audioMan->getTrackByIndex(i));
        for (int ar = 0; ar < audioMan->getTrackByIndex(i)->getAudioRegionListCount(); ar++) {
            AudioRegion* audioRegion = audioMan->getTrackByIndex(i)->getAudioRegionByIndex(ar);
            arrangeWidget->tl->addRegion(audioRegion);
            regionsToBeLoaded = regionsToBeLoaded + 1;
            audioRegion->loadFile(audioRegion->preLoadedFile, false);

        }
    }

    if (regionsToBeLoaded != 0) {
        dialogs::ProgressDialog::show(0, regionsToBeLoaded, "Loading project audio files...");
    }



    debug::out(3, "Finished parsing!");
}

bool MainWindow::ensureSaved() {
    if (isProjectEdited() == true) {
        switch (dialogs::MessageDialog::show("Save Changes?", "Do you want to save changes before you procceed?",
                                             dialogs::MessageDialog::icons::caution,
                                             dialogs::MessageDialog::buttons::yesNoCancel)) {
        case 0:
            return false;
        break;

        case 3:
            return true;
        break;
        case 2:
            saveProject(loadedProjectPath);
            return true;
        break;

        }
    } else {
        return true;
    }
}

bool MainWindow::isProjectEdited() {
    bool comparison = false;
    if (loadedProjectPath != "Untitled") {
        comparison = serialization->compaire(loadFile(loadedProjectPath).toStdString(), serialization->serialize(*audioMan, false));
    } else {
        comparison = serialization->compaire(untitledJSON.toStdString(), serialization->serialize(*audioMan, false));
    }
    return !comparison;
}

QString MainWindow::loadFile(QString path) {
    QFile loadedProjectFile(path);
    if(loadedProjectFile.open(QIODevice::ReadOnly)) {
        return loadedProjectFile.readAll();
    } else {
        dialogs::MessageDialog::show("Cannot open loaded project", "An error occoured whilst trying to load this file. This could be due to permission issues.",
                                     dialogs::MessageDialog::icons::no,
                                     dialogs::MessageDialog::buttons::okOnly);
        return "";
    }
}

void MainWindow::closeEvent(QCloseEvent *e) {
    if (ensureSaved() == true) {
        session->closeSession();
        e->accept();
    } else {
        e->ignore();
    }
}

void MainWindow::on_actionArrange_toggled(bool arg1)
{
    arrangeWidget->setVisible(arg1);
}

void MainWindow::on_actionMixer_toggled(bool arg1)
{
    mixerWidget->setVisible(arg1);
}

void MainWindow::on_actionConnect_to_Session_2_triggered()
{
    if (ensureSaved() == true) {
        if (session->getActive() == true) {
            if (dialogs::MessageDialog::show("Session already active", "A session is currently active. Do you want to disconnect and continue?",
                                             dialogs::MessageDialog::icons::caution,
                                             dialogs::MessageDialog::buttons::yesNo) == 2) {

            }
        }
        newProject();
        rtcClient = new RTCClientWindow(this, session, uac);
        rtcClient->show();
    }
}

void MainWindow::on_actionManage_Live_Session_triggered()
{
    rtcHost = new RTCHostWindow(this, session, uac);
    rtcHost->show();
}

void MainWindow::on_liveButton_clicked()
{
    QMenu menu(this);

    if (session->getActive() == true) {
        menu.addAction("ID: " + uac->sessionID)->setEnabled(false);
        menu.addAction("Password: " + uac->sessionPassword)->setEnabled(false);
        menu.addSeparator();
        QAction *chooseColor = new QAction("Disconnect", this);
        menu.addAction(chooseColor);
        connect(chooseColor, &QAction::triggered, session, &Session::disconnectSession);

    }

    menu.setWindowFlags(menu.windowFlags() | Qt::CustomizeWindowHint);
    menu.exec(cursor().pos());
    //menu.addAction("New Bus...");

    //menu.addAction("Cut")->setShortcut(QKeySequence::Cut);
    //menu.addAction("Copy")->setShortcut(QKeySequence::Copy);
    //menu.addAction("Paste")->setShortcut(QKeySequence::Copy);
    //menu.addSeparator();
    //menu.addAction("Rename...");
}

void MainWindow::on_actionRender_Audio_triggered()
{
    RenderWindow *renderWindow = new RenderWindow(this, &*audioMan);
    renderWindow->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    //audioMan->renderAudio(this, 48000, 2);
    serialization->sessionID = "testSession";
    serialization->copyToTemp = true;
    serialization->serialize(*audioMan, false);
}

void MainWindow::on_tempo_lcd_valueChanged(double arg1)
{
    audioMan->setBPM(arg1);
}
