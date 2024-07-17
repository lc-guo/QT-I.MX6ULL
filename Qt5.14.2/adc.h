#ifndef ADC_H
#define ADC_H

#include <QMainWindow>
#include <QFile>
#include <QChartView>
#include <QtCharts>
#include <QValueAxis>

namespace Ui { class adc; }

class adc : public QMainWindow
{
    Q_OBJECT
public:
    explicit adc(QWidget *parent = nullptr);
    ~adc();

    void adc_create_chart();
    void adc_update_chart(QSplineSeries *Series, QValueAxis *Axis, float add_value);
    void getADCValue(QFile &raw_file, QFile &scale_file);
    void handleMarkerClicked();

signals:

private slots:
    void on_adc_back_clicked();
    void adc_timer_timeout();

private:
    Ui::adc *ui;
    QTimer *adc_timer;
    QFile adc_raw_file;
    QFile adc_scale_file;
    float adc_value;

    QChart *chart;
    QValueAxis *mAxX;
    QValueAxis *mAxY;
    QSplineSeries *mADCSeries;
    QLegend *mlegend;



};

#endif // ADC_H
