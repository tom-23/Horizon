#ifndef TRACKINFOWIDGET_H
#define TRACKINFOWIDGET_H

#include <QWidget>

namespace Ui {
class TrackInfoWidget;
}

class TrackInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrackInfoWidget(QWidget *parent = nullptr);
    ~TrackInfoWidget();

private:
    Ui::TrackInfoWidget *ui;
};

#endif // TRACKINFOWIDGET_H
