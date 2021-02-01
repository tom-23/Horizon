#ifndef GUIUPDATETHHREAD_H
#define GUIUPDATETHHREAD_H

#include <QThread>
#include <QTimer>
#include <QObject>
#include <QDebug>

class GuiUpdateThread : public QThread
{
    Q_OBJECT
public:
    explicit GuiUpdateThread(QObject *parent = nullptr, std::function<void ()> updateFunction = nullptr);
    void run() override;

private:
    QTimer *uiUpdateTimer;
    std::function<void ()> updateFunction;
    int val = 0;
    void uiTimeOut();
};

#endif // GUIUPDATETHHREAD_H
