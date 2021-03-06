#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QDialog>

namespace Ui {
class SplashScreen;
}

class SplashScreen : public QDialog
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = nullptr);
    ~SplashScreen();

    void setText(QString text);
    void setVersion(QString version);

private:
    Ui::SplashScreen *ui;
};

#endif // SPLASHSCREEN_H
