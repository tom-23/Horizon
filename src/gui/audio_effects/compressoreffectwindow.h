#ifndef COMPRESSOREFFECTWINDOW_H
#define COMPRESSOREFFECTWINDOW_H

#include <QWidget>
#include "gui/ui_controls/horizondial.h"

namespace Ui {
class CompressorEffectWindow;
}

class CompressorEffectWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CompressorEffectWindow(QWidget *parent = nullptr);
    ~CompressorEffectWindow();

private:
    Ui::CompressorEffectWindow *ui;
};

#endif // COMPRESSOREFFECTWINDOW_H
