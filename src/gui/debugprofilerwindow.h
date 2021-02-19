#ifndef DEBUGPROFILERWINDOW_H
#define DEBUGPROFILERWINDOW_H

#include <QDialog>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QPainter>
#include <QGraphicsLayout>
#include <QTimer>
#include "common/dialogs.h"

using namespace QtCharts;

namespace Ui {
class DebugProfilerWindow;
}

class DebugProfilerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DebugProfilerWindow(QWidget *parent = nullptr);
    ~DebugProfilerWindow();

private slots:
    void on_startButton_toggled(bool checked);

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::DebugProfilerWindow *ui;
    QTimer *profilerTimer;
};

#endif // DEBUGPROFILERWINDOW_H
