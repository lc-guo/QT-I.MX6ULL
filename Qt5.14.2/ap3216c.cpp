#include "ap3216c.h"
#include "ui_ap3216c.h"
#include "mainwindow.h"

ap3216c::ap3216c(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ap3216c)
{
    ui->setupUi(this);
    this->setWindowTitle("Ap3216c");
    this->setFixedSize(800,480);

    ap3216c_timer = new QTimer();
#if __arm__
    system("cd /lib/modules/4.1.15");
    system("depmod");
    system("modprobe ap3216c.ko");
#endif

    connect(ap3216c_timer, SIGNAL(timeout()), this, SLOT(ap3216c_timer_timeout()));

#if __arm__
    fd = open(filename, O_RDWR);
    if(fd < 0) {
        printf("can't open file %s\r\n", filename);
        return;
    }
#endif

    ap3216c_create_chart();

    ap3216c_timer->start(200);
}


ap3216c::~ap3216c()
{
    delete ap3216c_timer;
    delete ui;
}

void ap3216c::ap3216c_create_chart()
{
    chart = new QChart();
    mAxY = new QValueAxis();

    // y轴范围
    mAxY->setRange(0, 1024);

    // Y轴分等份
    mAxY->setTickCount(9);
    mAxY->setLabelFormat("%d");

    chart->addAxis(mAxY, Qt::AlignLeft);

    ALSBar = new QBarSet("ALS");
    PSBar = new QBarSet("PS");
    IRBar = new QBarSet("IR");

    *ALSBar << 0;
    *PSBar << 0;
    *IRBar << 0;

    mBarseries = new QBarSeries();
    mBarseries->append(ALSBar);
    mBarseries->append(PSBar);
    mBarseries->append(IRBar);

    mBarseries->attachAxis(mAxY);

    QFont font = chart->legend()->font();
    font.setPointSizeF(20);
    chart->legend()->setFont(font);


    chart->addSeries(mBarseries);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // 将图表绑定到视图 wiget 为 QChartView
    ui->ap3216c_chart->setChart(chart);

}

void ap3216c::get_ap3216c_data()
{
    int ret = 0;
    ret = read(fd, databuf, sizeof(databuf));
    if(ret == 0) { 			/* 数据读取成功 */
        IR =  databuf[0]; 	/* ir传感器数据 */
        ALS = databuf[1]; 	/* als传感器数据 */
        PS =  databuf[2]; 	/* ps传感器数据 */
    }

    printf("IR=%d, ALS=%d, PS=%d\r\n", IR, ALS, PS);
}


void ap3216c::ap3216c_timer_timeout()
{
#if __arm__
    get_ap3216c_data();
    // mBarseries->detachAxis(mAxY);
    mBarseries->attachAxis(mAxY);
    ALSBar->insert(0,ALS);
    PSBar->insert(0,PS);
    IRBar->insert(0,IR);
    ui->ap3216c_chart->update();
#else
    ALS = QRandomGenerator::global()->bounded(1024);
    PS = QRandomGenerator::global()->bounded(1024);
    IR = QRandomGenerator::global()->bounded(1024);

    // mBarseries->detachAxis(mAxY);
    mBarseries->attachAxis(mAxY);

    ALSBar->insert(0,ALS);
    PSBar->insert(0,PS);
    IRBar->insert(0,IR);

    ui->ap3216c_chart->update();
#endif
}

void ap3216c::on_ap3216c_back_clicked()
{
    printf("on_ap3216c_back_clicked\r\n");
    ap3216c_timer->stop();

#if __arm__
    system("rmmod ap3216c.ko");
#endif

    this->close();
    MainWindow *m = new MainWindow();
    m->show();
}
