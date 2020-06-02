#include "thememanager.h"
#include <QDebug>
#include <QMessageBox>
#include <QJsonObject>

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
