#include "gpio.h"
#include "ui_gpio.h"
#include "mainwindow.h"
#include <linux/input.h>

gpio::gpio(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::gpio)
{
    ui->setupUi(this);
    this->setWindowTitle("GPIOS");
    this->setFixedSize(800,480);

#ifdef __arm__
    system("echo none > /sys/class/leds/red_led/trigger");
    system("echo none > /sys/class/leds/beep/trigger");

    led_file.setFileName("/sys/class/leds/red_led/brightness");
    if (!led_file.exists())
        printf("no find led device\r\n");

    beep_file.setFileName("/sys/class/leds/beep/brightness");
    if (!beep_file.exists())
        printf("no find beep device\r\n");
#endif
}


gpio::~gpio()
{
    delete ui;
}

void gpio::setGpioState(QFile &file, bool value)
{
    /* 获取当前状态 */
    bool now_state = getGpioState(file);

    /* 如果文件不存在，则返回 */
    if (!file.exists())
        return;

    if(!file.open(QIODevice::ReadWrite))
        qDebug()<<file.errorString();

    QByteArray buf[2] = {"0", "1"};

    /* 写 1 点亮 LED */
    if (value && !now_state)
        file.write(buf[1]);
    /* 写 0 熄灭 LED */
    else if(now_state)
        file.write(buf[0]);

    /* 关闭文件 */
    file.close();
}

bool gpio::getGpioState(QFile &file)
{
    /* 如果文件不存在，则返回 */
    if (!file.exists())
        return false;

    if(!file.open(QIODevice::ReadWrite))
        qDebug()<<file.errorString();

    QTextStream in(&file);

    /* 读取文件所有数据 */
    QString buf = in.readLine();

    /* 打印出读出的值 */
    qDebug()<< file.fileName() << " = " << buf << endl;

    file.close();
    if (buf == "1") {

        return true;
    } else {

        return false;
    }
}

void gpio::on_gpio_back_clicked()
{
    printf("on_gpio_back_clicked\r\n");
    setGpioState(led_file, 0);
    setGpioState(beep_file, 0);

    this->close();
    MainWindow *m = new MainWindow();
    m->show();
}

void gpio::on_beep_clicked(bool checked)
{
    printf("on_beep_clicked:%d\r\n", checked);

#ifdef __arm__
    setGpioState(beep_file, checked);
#endif

}

void gpio::on_led_clicked(bool checked)
{
    printf("on_led_clicked:%d\r\n", checked);
#ifdef __arm__
    setGpioState(led_file, checked);
#endif
}


void gpio::keyPressEvent(QKeyEvent *event)
{
    printf("keyPressEvent\r\n");

#ifdef __arm__
    if(event->key() == Qt::Key_VolumeDown) {
        printf("Key_Enter Press\r\n");
        ui->key->setChecked(true);
    }
#else
    if(event->key() == Qt::Key_Down) {
        printf("Key_Enter Press\r\n");
        ui->key->setChecked(true);
    }
#endif

    QWidget::keyPressEvent(event);

}

void gpio::keyReleaseEvent(QKeyEvent *event) {
#ifdef __arm__
    if(event->key() == Qt::Key_VolumeDown) {
        printf("Key_Enter Release\r\n");
        ui->key->setChecked(false);
    }
#else
    if(event->key() == Qt::Key_Down) {
        printf("Key_Enter Press\r\n");
        ui->key->setChecked(false);
    }
#endif
    QWidget::keyReleaseEvent(event);
}
