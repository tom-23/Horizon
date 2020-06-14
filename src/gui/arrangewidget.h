#ifndef ARRANGEWIDGET_H
#define ARRANGEWIDGET_H

#include <QWidget>
//#include "trackcontrolswidget.h"

//#include "audioregion.h"
//#include "region.h"
//#include "track.h"

#include "app/audiomanager.h"

class Track;

namespace Ui {
class ArrangeWidget;
}

class ArrangeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ArrangeWidget(QWidget *parent);
    ~ArrangeWidget();

    Timeline *tl;

    void setAudioManager(AudioManager &_audioMan);

private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_zoomSlider_valueChanged(int value);
    void on_pushButton_clicked();

private:
    Ui::ArrangeWidget *ui;
    AudioManager *audioMan;
protected:
    void resizeEvent(QResizeEvent *event);

};

#endif // ARRANGEWIDGET_H
