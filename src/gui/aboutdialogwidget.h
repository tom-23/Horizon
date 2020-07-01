#ifndef AboutDialogWidget_H
#define AboutDialogWidget_H

#include <QDialog>

namespace Ui {
class AboutDialogWidget;
}

class AboutDialogWidget : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialogWidget(QWidget *parent = nullptr, QString svgPath = "");
    ~AboutDialogWidget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AboutDialogWidget *ui;
};

#endif // AboutDialogWidget_H
