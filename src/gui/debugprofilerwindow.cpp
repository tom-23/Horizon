#include "debugprofilerwindow.h"
#include "ui_debugprofilerwindow.h"

DebugProfilerWindow::DebugProfilerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugProfilerWindow)
{
    ui->setupUi(this);

    profilerTimer = new QTimer(this);

    QLineSeries *series = new QLineSeries();
    series->setColor("#ff0000");
    series->setName("AudioRegions");
    series->append(0, 6);
    series->append(1, 4);
    series->append(2, 8);
    series->append(3, 4);
    series->append(5, 5);
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();

    chart->setBackgroundBrush(QBrush(dialogs::getThemeManager()->getColor("background")));
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);

    QFont font = parent->font();
    font.setPixelSize(10);
    chart->axisX()->setLabelsVisible(false);
    chart->axisX()->setGridLineColor(dialogs::getThemeManager()->getColor("font-color-secondary"));
    chart->axisY()->setLabelsColor(dialogs::getThemeManager()->getColor("font-color-secondary"));
    chart->axisY()->setGridLineColor(dialogs::getThemeManager()->getColor("font-color-secondary"));
    chart->axisY()->setLabelsFont(font);


    QChartView *chartView = new QChartView(chart);

    chartView->setRenderHint(QPainter::Antialiasing);
    ui->barGraph->layout()->addWidget(chartView);
}

DebugProfilerWindow::~DebugProfilerWindow()
{
    delete ui;
}

void DebugProfilerWindow::on_startButton_toggled(bool checked)
{
    if (checked) {
        profilerTimer->start();
    } else {
        profilerTimer->stop();
    }
}

void DebugProfilerWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    int interval = arg1.toInt();
    if (interval != 0) {
        profilerTimer->setInterval(interval);
    }
}
