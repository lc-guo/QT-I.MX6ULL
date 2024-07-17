#ifndef AP3216C_H
#define AP3216C_H

#include <QMainWindow>
#include <QChartView>
#include <QtCharts>
#include <QValueAxis>
#include <QTimer>
#include <QRandomGenerator>

QT_CHARTS_USE_NAMESPACE;

namespace Ui { class ap3216c; }

class ap3216c : public QMainWindow
{
    Q_OBJECT
public:
    explicit ap3216c(QWidget *parent = nullptr);
    ~ap3216c();
    void ap3216c_create_chart();
    void get_ap3216c_data();

private:
    Ui::ap3216c *ui;
    QTimer *ap3216c_timer;
    int fd;
    char const *filename = "/dev/ap3216c";

    QChart *chart;
    QValueAxis *mAxY;
    QBarSet *ALSBar;
    QBarSet *PSBar;
    QBarSet *IRBar;
    QBarSeries *mBarseries;
    unsigned short ALS;
    unsigned short PS;
    unsigned short IR;
    unsigned short databuf[3];




signals:

private slots:
    void on_ap3216c_back_clicked();
    void ap3216c_timer_timeout();



};

#endif // AP3216C_H
