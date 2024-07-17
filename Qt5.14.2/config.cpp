#include "config.h"
#include "mainwindow.h"
#include "ui_config.h"

extern int brightness;


config::config(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::config)
{
    ui->setupUi(this);
    this->setWindowTitle("Config");
    this->setFixedSize(800,480);

#ifdef __arm__
    brightness_file.setFileName("/sys/devices/platform/backlight/backlight/backlight/brightness");
    if (!brightness_file.exists())
        printf("no find brightness device\r\n");

    if(!brightness_file.open(QIODevice::ReadWrite))
        qDebug()<<brightness_file.errorString();

    QTextStream in(&brightness_file);

    /* 读取文件所有数据 */
    QString buf = in.readLine();

    /* 打印出读出的值 */
    qDebug()<< brightness_file.fileName() << " = " << buf << endl;
    brightness_file.close();

    ui->brightness_slider->setValue(buf.toInt()-1);
    brightness = buf.toInt();
#endif

}


config::~config()
{
    delete ui;
}

void config::setBrightness(QFile &file, int value)
{
    /* 如果文件不存在，则返回 */
    if (!file.exists())
        return;

    if(!file.open(QIODevice::ReadWrite))
        qDebug()<<file.errorString();

    QByteArray buf[7] = {"1", "2", "3", "4", "5", "6", "7"};
    file.write(buf[value]);
    printf("set brightness = %d\r\n", buf[value].toInt());

    /* 关闭文件 */
    file.close();
}


void config::on_config_back_clicked()
{
    printf("on_config_back_clicked\r\n");
    this->close();
    MainWindow *m = new MainWindow();
    m->show();
}

void config::on_brightness_slider_valueChanged(int value)
{
#ifdef __arm__
    setBrightness(brightness_file, value);
    brightness = value+1;
#else
    printf("set brightness = %d\r\n", value+1);
    brightness = value+1;
#endif

    QString fileName = QString(":/images/brightness_%1.svg").arg(brightness - 1);
    QPixmap pixmap(fileName);
    ui->brightness->setPixmap(pixmap);
    ui->brightness->setFixedSize(64, 64);
}


void config::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);

    QString fileName = QString(":/images/brightness_%1.svg").arg(brightness - 1);
    QPixmap pixmap(fileName);
    ui->brightness->setPixmap(pixmap);
    ui->brightness->setFixedSize(64, 64);
    ui->brightness_slider->setValue(brightness - 1);
}
