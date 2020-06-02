#ifndef TRACKCONTROLSWIDGET_H
#define TRACKCONTROLSWIDGET_H

#include <QWidget>

namespace Ui {
class TrackControlsWidget;
}

class TrackControlsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrackControlsWidget(QWidget *parent = nullptr);
    ~TrackControlsWidget();

private slots:


    void on_TrackControlsWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::TrackControlsWidget *ui;

};

#endif // TRACKCONTROLSWIDGET_H
