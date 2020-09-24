#ifndef LoadingDialogWidget_H
#define LoadingDialogWidget_H

#include <QDialog>

namespace Ui {
class LoadingDialogWidget;
}

class LoadingDialogWidget : public QDialog
{
    Q_OBJECT

public:
    LoadingDialogWidget(QWidget *parent = nullptr, int min = 0, int max = 100, QString message = "Loading...");
    ~LoadingDialogWidget();

    void updateProgress(int value);




private:
    Ui::LoadingDialogWidget *ui;

};

#endif // LoadingDialogWidget_H
