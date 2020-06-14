#include "thememanager.h"

ThemeManager::ThemeManager(QWidget *_parent, QString _themeFileName)
{
    parent = _parent;
    this->setThemeFile(_themeFileName);

}

void ThemeManager::setThemeFile(QString _themeFileName) {

    themeFileName = _themeFileName;
    debug::out(3, "Loading theme...");

    QFile styleQSS(":/base/styles.qss");
    QFile themeFile(themeFileName);

    if(!styleQSS.open(QIODevice::ReadOnly)) {
        debug::out(1, styleQSS.errorString().toStdString());
    }

    if(!themeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        debug::out(1, themeFile.errorString().toStdString());
    }

    QTextStream styleQSSTS(&styleQSS);
    QJsonDocument themeFileJSON = QJsonDocument::fromJson(themeFile.readAll());
    QJsonObject *themeFileObject = new QJsonObject(themeFileJSON.object());
    colorTheme = themeFileObject;

    QString finalQSS;

    while(!styleQSSTS.atEnd()) {
        QString line = styleQSSTS.readLine();


        foreach(const QString &key, themeFileObject->keys()) {
            line = line.replace(QString("@" + key), QString(themeFileObject->value(key).toString()));

        }

        finalQSS.append(line);

    }

    parent->setStyleSheet(finalQSS);
    parent->update();
    parent->repaint();

    debug::out(3, "Theme is successfully parsed!");

}

QColor ThemeManager::getColor(QString key) {
    //qDebug() << "Value:" << colorTheme->value("borders").toString();
    return QColor(colorTheme->value(key).toString());
}

void ThemeManager::updateTheme() {
    this->setThemeFile(themeFileName);
}


QString ThemeManager::colorizeSVG(QString filename) {
    QFile baseSVG(filename);
    QFileInfo fileInfo(baseSVG.fileName());
    if(!baseSVG.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", baseSVG.errorString());

    }
    QTextStream baseSVGTS(&baseSVG);
    QString finalSVG;

    while (!baseSVGTS.atEnd()) {
        QString line = baseSVGTS.readLine();
        foreach(const QString &key, colorTheme->keys()) {
            line = line.replace(QString("@" + key), QString(colorTheme->value(key).toString()));
        }
        finalSVG.append(line);
    }

    QString tempDirLocation;

    #ifdef _WIN32

    if (!QDir("/Temp").exists()) {
       QDir().mkdir("/Temp");
    }
    tempDirLocation = "/Temp";


    #else
    QString appBundlePath = QString::fromUtf8(util::getResourceBundle().c_str());
    if (!QDir(appBundlePath + "/temp").exists()) {
       QDir().mkdir(appBundlePath + "/temp");
    }
    tempDirLocation = appBundlePath + "/temp";
    #endif

    QString savedFileName = tempDirLocation + "/theme_" + fileInfo.fileName();

    QFile outSVG(savedFileName);


    if (outSVG.open(QIODevice::ReadWrite) )
    {
        outSVG.write(finalSVG.toUtf8());
    }

    debug::out(3, "Returned colorized theme_" + fileInfo.fileName().toStdString());

    return savedFileName;
}
