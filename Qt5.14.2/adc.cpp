#include "adc.h"
#include "mainwindow.h"
#include "ui_adc.h"

adc::adc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::adc)
{
    ui->setupUi(this);
    this->setWindowTitle("ADC");
    this->setFixedSize(800,480);

    adc_timer = new QTimer();

#ifdef __arm__
    adc_raw_file.setFileName("/sys/bus/iio/devices/iio:device0/in_voltage1_raw");
    if (!adc_raw_file.exists()){
        printf("no find adc device\r\n");
        QMessageBox::warning(this, "Open failed", "can't open adc_raw_file\r\n");
    }

    adc_scale_file.setFileName("/sys/bus/iio/devices/iio:device0/in_voltage_scale");
    if (!adc_scale_file.exists()){
        printf("no find adc device\r\n");
        QMessageBox::warning(this, "Open failed", "can't open adc_scale_file\r\n");
    }
#endif

    adc_create_chart();

    connect(adc_timer, SIGNAL(timeout()), this, SLOT(adc_timer_timeout()));

    adc_timer->start(100);
}


adc::~adc()
{
    delete adc_timer;
    delete ui;
}


void adc::getADCValue(QFile &raw_file, QFile &scale_file)
{
    /* 如果文件不存在，则返回 */
    if (!raw_file.exists() || !scale_file.exists())
        return;

    if(!raw_file.open(QIODevice::ReadOnly))
        qDebug()<<raw_file.errorString();

    if(!scale_file.open(QIODevice::ReadOnly))
        qDebug()<<scale_file.errorString();

    /* 获取原始 ADC 值 */
    QTextStream raw_in(&raw_file);
    QString raw_buf = raw_in.readLine();
    qDebug()<< raw_file.fileName() << " = " << raw_buf << endl;
    raw_file.close();

     /* 获取 ADC 尺度 */
    QTextStream scale_in(&scale_file);
    QString scale_buf = scale_in.readLine();
    qDebug()<< scale_file.fileName() << " = " << scale_buf << endl;
    scale_file.close();

    adc_value = raw_buf.toFloat() * scale_buf.toFloat() / 1000;
    printf("adc value = %f\r\n", adc_value);
}



void adc::adc_create_chart()
{
    chart = new QChart();
    mAxY = new QValueAxis();
    mAxX = new QValueAxis();
    mADCSeries = new QSplineSeries();

    // x,y轴范围
    mAxX->setRange(0, 10);
    mAxY->setRange(0, 3.3);

    // x,Y轴分等份
    mAxX->setTickCount(11);
    mAxY->setTickCount(10);

    // 将系列添加到图表
    chart->addSeries(mADCSeries);
    chart->setTheme(QtCharts::QChart::ChartThemeLight);

    mAxX->setTitleText(QString(tr("time")));
    mAxY->setTitleText(QString(tr("Voltage")));
    chart->addAxis(mAxX, Qt::AlignBottom);
    chart->addAxis(mAxY, Qt::AlignLeft);

    mADCSeries->attachAxis(mAxY);
    mADCSeries->attachAxis(mAxX);


    //隐藏背景
    //chart->setBackgroundVisible(true);
    //设置外边界全部为0
    //chart->setContentsMargins(0, 0, 0, 0);
    //设置内边界全部为0
    //chart->setMargins(QMargins(0, 0, 0, 0));
    //设置背景区域无圆角
    //chart->setBackgroundRoundness(0);

    //突出曲线上的点
    mADCSeries->setPointsVisible(true);

    //图例
    mlegend = chart->legend();
    mADCSeries->setName("GPIO1_IO01");
    mADCSeries->setColor(QColor(255,0,0));

    //在底部显示
    mlegend->setAlignment(Qt::AlignBottom);
    mlegend->show();


    QFont font = mlegend->font();
    font.setPointSizeF(20);
    mlegend->setFont(font);

    // 将图表绑定到视图 wiget 为 QChartView
    ui->adc_chart->setChart(chart);

    const auto markers = chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, &QLegendMarker::clicked, this ,&adc::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &adc::handleMarkerClicked);
    }
}


//点击事件
void adc::handleMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    //断言
    Q_ASSERT(marker);
    switch (marker->type())
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
        //控序列隐藏/显示
        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);

        //修改图例
        // Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!marker->series()->isVisible())
            alpha = 0.5;

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);
        break;
        }
    default:
        {
        qDebug() << "Unknown marker type";
        break;
        }
    }
}

void adc::adc_update_chart(QSplineSeries *Series, QValueAxis *Axis, float add_value)
{
    // 如果数据序列中的值数量超过了 x 轴设置的最大值 10
    if(Series->count() > Axis->max())
    {
        // 将序列数据中的前 10 个值依次向前挪一个
        for(int i=0; i<Series->count()-1; ++i)
        {
            QPointF point = Series->at(i+1);
            Series->replace(i, point.x() -1, point.y());
        }
        // 在最后添加新的点
        Series->replace(Axis->max(), Axis->max(), add_value);
    }
    // 如果数据序列中的值没超过 x 轴设置的最大值 10 ，就正常添加点
    else
    {
        QPointF point;
        point.setX(Series->count());
        point.setY(add_value);
        Series->append(point);
    }
}

void adc::adc_timer_timeout()
{
#if __arm__
    getADCValue(adc_raw_file, adc_scale_file);
    adc_update_chart(mADCSeries, mAxX, adc_value);
#else
    adc_value = QRandomGenerator::global()->bounded(0,3);
    adc_update_chart(mADCSeries, mAxX, adc_value);
#endif
}

void adc::on_adc_back_clicked()
{
    printf("on_adc_back_clicked\r\n");
    adc_timer->stop();

    this->close();
    MainWindow *m = new MainWindow();
    m->show();
}
