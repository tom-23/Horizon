#include "thememanager.h"
#include <QDebug>
#include <QMessageBox>
#include <QJsonObject>
#include <QDir>

ThemeManager::ThemeManager(QWidget *_parent, QString _themeFileName)
{
    parent = _parent;
    this->setThemeFile(_themeFileName);

}

void ThemeManager::setThemeFile(QString _themeFileName) {

    themeFileName = _themeFileName;

    QFile styleQSS(":/base/styles.qss");
    QFile themeFile(themeFileName);

    if(!styleQSS.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", styleQSS.errorString());
    }

    if(!themeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(0, "error", styleQSS.errorString() + " with opening theme definition");
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

    #if defined(Q_OS_WIN)

    if (!QDir("/Temp").exists()) {
       QDir().mkdir("/Temp");
    }
    tempDirLocation = "/Temp";

    #elif defined(Q_OS_OSX)
    if (!QDir("../Resources/temp").exists()) {
       QDir().mkdir("../Resources/temp");
    }
    tempDirLocation = "../Resources/temp";
    #endif

    QString savedFileName = tempDirLocation + "/theme_" + fileInfo.fileName();

    QFile outSVG(savedFileName);
    qDebug() << finalSVG;

    if (outSVG.open(QIODevice::ReadWrite) )
    {
        outSVG.write(finalSVG.toUtf8());
    }

    return savedFileName;
}
