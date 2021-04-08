#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, SplashScreen *splashScreen, Preferences *_prefs)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString themeLoc;

    // unimplemented feature
    ui->footer->hide();

    // set the default project to an untitled project
    loadedProjectPath = "Untitled";
    prefs = _prefs;

    // here we get the directory which holds all the theme files. We do the the same way we do with the preferences.
    // See Horizon.cpp
    #ifndef _WIN32
        themeLoc = QString::fromUtf8(util::getResourceBundle().c_str()) + "/themes/" + prefs->themeName + ".json";
    #else
        themeLoc = QString::fromUtf8(util::getInstallDir().c_str()) + "/themes/" + prefs->themeName + ".json";
    #endif

    // initialise a new theme manager.
    themeMan = new ThemeManager(this, themeLoc);
    dialogs::setParent(this);
    dialogs::setThemeManager(themeMan);

    updateIconThemes();

    // todo: implement this
    colorPicker = new ColorPickerWidget(this);

    splashScreen->setText("Loading audio engine...");
    debug::out(3, "Loading audio manager...");

    // initalize some object here.
    infoWidget = new InfoWidget(this);

    arrangeWidget = new ArrangeWidget(this);
    // create a new audio manager class and store it in a unique smart pointer. smart pointers are great compared to
    // raw points as they handle garbage collection.
    audioMan = new AudioManager(this, *arrangeWidget->tl);
    splashScreen->setText("Starting HorizonEngine...");
    debug::out(3, "Starting HorizonEngine...");
    audioMan->initHorizonEngine();
    audioMan->initSocket();

    // realtime collab session
    session = audioMan->session;

    // give the arrange widget a pointer ref to the audio man.
    arrangeWidget->setAudioManager(*audioMan);

    mixerWidget = new MixerWidget(this);

    // give the arrange widget a pointer ref to the mixer object.
    arrangeWidget->setMixer(mixerWidget->mixer);

    libraryWidget = new LibraryWidget(this, prefs, arrangeWidget);



    splashScreen->setText("Initialising UAC...");
    debug::out(3, "Initialising UAC...");

    // create a new UAC.
    uac = new UAC();

    // here we are setting some defaults. Like bpm, bar division and lookahead time.


    debug::out(3, "Loaded audio manager with default settings.");

    // add stuff to the ui.
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(5);
    mainLayout->setSpacing(5);
    mainLayout->addWidget(infoWidget);

    QVBoxLayout *mixerArrangeLayout = new QVBoxLayout;
    mixerArrangeLayout->addWidget(arrangeWidget);
    mixerArrangeLayout->addWidget(mixerWidget);

    mainLayout->addLayout(mixerArrangeLayout);

    mainLayout->addWidget(libraryWidget);

    ui->content->setLayout(mainLayout);

    arrangeWidget->show();
    arrangeWidget->tl->setColorTheme(themeMan);

    mixerWidget->show();
    if (!prefs->showMixer) {
        ui->actionMixer->setChecked(false);
    }

    libraryWidget->show();
    if (!prefs->showLibrary) {
        ui->actionLibrary->setChecked(false);
    }

    // here we start the update thread which manage the update of moving ui components (meters, playhead)...
    updateThread = new GuiUpdateThread(this, std::bind(&MainWindow::uiUpdate, this));


    splashScreen->setText("Creating new project...");
    serialization = new ProjectSerialization();

    //make a new project.
    newProject();

    // kill splash screen
    splashScreen->close();

    // remove the focus box on macOS (ODC ui stuffs)
    ui->tempo_lcd->setAttribute(Qt::WA_MacShowFocusRect, 0);

    // show the ui full screen. (no brainer)
    this->showMaximized();
    // start the update thread.
    updateThread->run();

#ifndef _WIN32
    // on macOS, enable touch bar support (WIP)
   // util::macInitTouchbar(this);
#endif
    debug::out(3, "MainWindow Init Done!");
}

void MainWindow::uiUpdate() {
    // TODO: rewrite ui updating code. (event driven)
    // this function handles ui updating. Whilst in hindsight, this is probably horrible and could be accomplished
    // using Qt's slots and signals, it gets the job done for the most part.
    float currentGridTime = audioMan->getCurrentGridTime();

    // if we've reached the end of the project, stop playback and rewind.
    if (currentGridTime >= arrangeWidget->tl->barCount + 1) {
        audioMan->stop();
        arrangeWidget->tl->setPlayheadLocation(0.0);
    }

    // update the playhead's location to the current time.
    arrangeWidget->tl->setPlayheadLocation(audioMan->getCurrentGridTime());

    // TODO: refactor this. forogtten what it does. maybe i'll work it out one day. (1/2/2021)
    float glr = float(floor(currentGridTime * arrangeWidget->tl->barLength)) / arrangeWidget->tl->barLength;
    ui->barNumberLabel->setText(QString::number(floor(currentGridTime)));
    ui->beatNumberLabel->setText(QString::number(((glr - floor(currentGridTime)) * arrangeWidget->tl->barLength) + 1));

    //QApplication::processEvents();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playButton_clicked()
{
    // if the play button is clicked, toggle the playback
    // TODO: create a pause button for better visual feedback.
    togglePlayback();
}

void MainWindow::on_actionAbout_triggered()
{
    // show the about dialog when the user clicks them menu button.
    new dialogs::AboutDialog();
}

void MainWindow::on_stopButton_clicked()
{
    // when the stop button is clicked, stop playback and set the playhead location to 0.0;
    audioMan->stop();
    arrangeWidget->tl->setPlayheadLocation(0.0);
}

void MainWindow::updateIconThemes() {
    // when this function is called, we set the different ui buttons to "colorized" versions of them selves relitive to
    // the current theme.
    ui->playButton->setStyleSheet("image: url('" + themeMan->colorizeSVG(":/svg/svg/play.svg") + "');");
    ui->stopButton->setStyleSheet("image: url('" + themeMan->colorizeSVG(":/svg/svg/stop.svg") + "');");
    ui->recordButton->setStyleSheet("image: url('" + themeMan->colorizeSVG(":/svg/svg/record.svg") + "');");
}

void MainWindow::on_actionPreferences_triggered()
{
    // show prefs window
    PreferencesWindow *pw = new PreferencesWindow(this, prefs);
    pw->show();
}

void MainWindow::togglePlayback() {
    // you can work this out by yourself.
    if (audioMan->isPlaying == false) {
        audioMan->play();
    } else {
        audioMan->pause();
    }
}

void MainWindow::keyPressEvent(QKeyEvent* ke) {
    switch (ke->key()) {
        case Qt::Key::Key_Space:
            // TODO: make this a menu bar option
            // when you hit the space bar, toggle playback.
            togglePlayback();
        break;

        case Qt::Key::Key_Alt:
            // disable region snapping when the alt key is down
            arrangeWidget->tl->regionSnapping = false;
        break;

    }
    QMainWindow::keyPressEvent(ke); // base class implementation
}

void MainWindow::keyReleaseEvent(QKeyEvent* ke) {
    switch (ke->key()) {
        case Qt::Key::Key_Alt:
            //re enable reigon snapping on key up.
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
    // generate a random color
    // TODO: use a color pallet instead of generating a random color.
    // this is bad as you may not be able to see the waveform if its selected.
    QColor color = QColor::fromRgb(QRandomGenerator::global()->generate());
    // make a new uuid
    QString uuid = QUuid::createUuid().toString();
    newTrack(color, uuid);
    // tell the realtime session to make a new track.
    session->newTrack(color.name(QColor::HexRgb), uuid);
}

void MainWindow::newTrack(QColor color, QString uuid) {
    // create a new track (passing a null pointer tells the function to create a new track)
    arrangeWidget->addAudioTrack(nullptr, uuid)->updateColor(color);
}

void MainWindow::on_actionNew_Project_triggered()
{
    // if the user wants to create a new project, make sure they've saved the currently open one.
    if (ensureSaved() == true) {
        newProject();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    // if the user wants to open a project, make sure they've saved the currently open one.
    if (ensureSaved() == true) {
        // if they have, show a project open dialog
        QFileDialog dialog(this, tr("Open Project"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                           tr("Horizon Project File (*.hzp)"));
        dialog.setParent(this);
        dialog.setWindowModality(Qt::WindowModal);

        dialog.exec();

        QString dialogFileName = dialog.selectedFiles()[0];

        if (dialogFileName != "") { // ensure a file was selected.
            openProject(dialogFileName);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    // save project with loaded path
    saveProject(QString::fromStdString(loadedProjectPath.toStdString()));
}

void MainWindow::on_actionSave_As_triggered()
{
    // save a project passing no path. (save as)
    saveProject();
}

void MainWindow::openProject(QString fileName) {
    // load the file
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly)) {
        // clear the audio manager lists.
        audioMan->clearAll();
        // clear the timeline ui
        arrangeWidget->tl->clearAll();
        // clear the mixer ui
        mixerWidget->mixer->clearAll();
        // load the project's json from the file's buffer.
        loadProjectJSON(inputFile.readAll());
        // set the window's title to the loaded file's file name.
        QFileInfo fileInfo(fileName);
        this->setWindowTitle(fileInfo.fileName());
        // set the loadedProjectPath global var.
        loadedProjectPath = fileName;

    } else {
        debug::out(3, "Cannot open project file. It is probably open in another program.");
    }
}

void MainWindow::saveProject(QString fileName) {
    if (fileName != "Untitled") {
        Bog toilet(fileName);
        if (toilet.open(QIODevice::ReadWrite) ) // open the file
        {
            ProjectSerialization *serialization = new ProjectSerialization;

            toilet.resize(0); // remove all file contents by resizing it. (probably not best practice but oh well)
            toilet.write(QString::fromStdString(serialization->serialize(*audioMan, false)).toUtf8()); // write the project's json.

            toilet.flush(); // flush the loo
            toilet.close(); // close the lid

            QFileInfo fileInfo(fileName);
            this->setWindowTitle(fileInfo.fileName());
            loadedProjectPath = fileName;

        } else {
            // if we can't write to a file, show an error.
            dialogs::MessageDialog::show("Cannot save file", "An error occurred whilst trying to load this file. This could be due to permission issues.",
                                         dialogs::MessageDialog::icons::no,
                                         dialogs::MessageDialog::buttons::okOnly);
        }

    } else {
        // if the file is untitled, force a save.
        QString dialogFileName = QFileDialog::getSaveFileName(this,
                tr("Save Project"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                tr("Horizon Project File (*.hzp)"));
        if (dialogFileName != "") {
            saveProject(dialogFileName);
        }

    }


}

void MainWindow::newProject() {
        // when set the project path to Unitled to indicate this is a new project
        loadedProjectPath = "Untitled";
        this->setWindowTitle(loadedProjectPath);
        // clear relevant classes
        audioMan->clearAll();
        arrangeWidget->tl->clearAll();
        mixerWidget->mixer->clearAll();
        // set blank json
        untitledJSON = QString::fromStdString(serialization->serialize(*audioMan, false));
}

void MainWindow::loadProjectJSON(QString JSON) {
    // set the tempo box to the current bpm
    ui->tempo_lcd->setValue(audioMan->getBPM());
    int regionsToBeLoaded = 0;
    for (int i = 0; i < audioMan->getTrackListCount(); i++) {
        arrangeWidget->addAudioTrack(audioMan->trackList->at(i));
        for (int ar = 0; ar < audioMan->trackList->at(i)->getAudioRegionListCount(); ar++) {
            regionsToBeLoaded = regionsToBeLoaded + 1;
        }
    }


    if (regionsToBeLoaded != 0) {
        dialogs::ProgressDialog::show(0, regionsToBeLoaded, "Loading project audio files...");
    }
    debug::out(3, "Loading project JSON...");
    serialization->deSerialize(JSON.toStdString(), *audioMan);
    for (int i = 0; i < audioMan->getTrackListCount(); i++) {
        arrangeWidget->addAudioTrack(audioMan->trackList->at(i));
        for (int ar = 0; ar < audioMan->trackList->at(i)->getAudioRegionListCount(); ar++) {
            AudioRegion* audioRegion = audioMan->trackList->at(i)->getAudioRegionByIndex(ar);
            arrangeWidget->tl->addRegion(audioRegion);
            regionsToBeLoaded = regionsToBeLoaded + 1;
            //audioRegion->loadFile(audioRegion->preLoadedFile, false);

        }
    }

    audioMan->pauseEngineCommunication = false;

    audioMan->sendCommand("loadProject", serialization->rootProject);

    qApp->processEvents();


    debug::out(3, "Finished parsing!");
}

bool MainWindow::ensureSaved() {
    // here is where we show a message box if the project is different to last time.
    // return true = project *saved*
    // return false = cancel
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
    }
    return true;
}

bool MainWindow::isProjectEdited() {
    // here is where the serialize the project and then comapaire it to the saved json. if they are the same
    // we have no changes if not, we have changed.
    bool comparison = false;
    if (loadedProjectPath != "Untitled") {
        comparison = serialization->compaire(loadFile(loadedProjectPath).toStdString(), serialization->serialize(*audioMan, false));
    } else {
        comparison = serialization->compaire(untitledJSON.toStdString(), serialization->serialize(*audioMan, false));
    }
    return !comparison;
}

QString MainWindow::loadFile(QString path) {
    // return a file's string. if we can't do that, show an error.
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
    // if the user closes the program, ensure the file is saved and also, destruct things like the UAC.
    if (ensureSaved() == true) {
        session->closeSession();
        uac->~UAC();
        audioMan->closeConnectionAndEngine();
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

void MainWindow::on_actionLibrary_toggled(bool arg1)
{
    libraryWidget->setVisible(arg1);
}

void MainWindow::on_actionPropery_Editor_toggled(bool arg1)
{
    infoWidget->setVisible(arg1);
}

void MainWindow::on_actionConnect_to_Session_2_triggered()
{
    // First, we make sure the current project is saved as we will close it later.
    if (ensureSaved() == true) {
        // then, we check if a session is already active. If so, show a message box.
        if (session->getActive() == true) {
            if (dialogs::MessageDialog::show("Session already active", "A session is currently active. Do you want to disconnect and continue?",
                                             dialogs::MessageDialog::icons::caution,
                                             dialogs::MessageDialog::buttons::yesNo) == 2) {
                // fixme: this message box will continue to disconnect a user even if they click the no button.
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
    // this is work in progress.
    // TODO: make the live button more useful.
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
    // TODO: this looks like debug code. it should probably go (1/3/2021)
    //audioMan->renderAudio(this, 48000, 2);
    serialization->sessionID = "testSession";
    serialization->copyToTemp = true;
    serialization->serialize(*audioMan, false);
}

void MainWindow::on_tempo_lcd_valueChanged(double arg1)
{
    audioMan->setBPM(arg1);
}

void MainWindow::on_actionColor_Picker_toggled(bool arg1)
{
    if (arg1) {
        colorPicker->show();
    } else {
        colorPicker->hide();
    }
}


void MainWindow::on_actionProfiler_triggered()
{
    DebugProfilerWindow *profiler = new DebugProfilerWindow(this);
    profiler->show();
}
