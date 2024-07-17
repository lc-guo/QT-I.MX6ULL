#include "uart.h"
#include "ui_uart.h"
#include "mainwindow.h"


uart::uart(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::uart)
{
    ui->setupUi(this);
    this->setWindowTitle("UART");
    this->setFixedSize(800,480);

    serialPort = new QSerialPort(this);
    ui->send->setEnabled(false);

    ui->rx_browser->setPlaceholderText("Received messages...");
    ui->tx_edit->setPlaceholderText("send message.....");
    ui->rx_browser->show();
    ui->tx_edit->show();

    /* 扫描系统的串口 */
    scanSerialPort();

    /* 波特率项初始化 */
    baudRateItemInit();

    /* 数据位项初始化 */
    dataBitsItemInit();

    /* 检验位项初始化 */
    parityItemInit();

    /* 停止位项初始化 */
    stopBitsItemInit();

    /* 串口有数据就显示 */
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialPortReadyRead()));
}

uart::~uart()
{
    delete serialPort;

    delete ui;
}

void uart::scanSerialPort()
{
    /* 查找可用串口 */
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->port->addItem(info.portName());
    }
}


void uart::baudRateItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"1200"<<"2400"<<"4800"<<"9600"
       <<"19200"<<"38400"<<"57600"
      <<"115200"<<"230400"<<"460800"
     <<"921600";
    for (int i = 0; i < 11; i++) {
        ui->rate->addItem(list[i]);
    }
    ui->rate->setCurrentIndex(7);
}

void uart::dataBitsItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"5"<<"6"<<"7"<<"8";
    for (int i = 0; i < 4; i++) {
        ui->data_bit->addItem(list[i]);
    }
    ui->data_bit->setCurrentIndex(3);
}

void uart::parityItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"None"<<"Even"<<"Odd"<<"Space"<<"Mark";
    for (int i = 0; i < 5; i++) {
        ui->check_bit->addItem(list[i]);
    }
    ui->check_bit->setCurrentIndex(0);
}


void uart::stopBitsItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"1"<<"2";
    for (int i = 0; i < 2; i++) {
        ui->stop_bit->addItem(list[i]);
    }
    ui->stop_bit->setCurrentIndex(0);
}



void uart::on_uart_back_clicked()
{
    printf("on_uart_back_clicked\r\n");
    serialPort->close();

    this->close();
    MainWindow *m = new MainWindow();
    m->show();
}

void uart::on_open_clicked()
{
    printf("on_open_clicked\r\n");
    if (ui->open->text() == "Open") {
          /* 设置串口名 */
          serialPort->setPortName(ui->port->currentText());
          /* 设置波特率 */
          serialPort->setBaudRate(ui->rate->currentText().toInt());
          /* 设置数据位数 */
          switch (ui->data_bit->currentText().toInt()) {
          case 5:
              serialPort->setDataBits(QSerialPort::Data5);
              break;
          case 6:
              serialPort->setDataBits(QSerialPort::Data6);
              break;
          case 7:
              serialPort->setDataBits(QSerialPort::Data7);
              break;
          case 8:
              serialPort->setDataBits(QSerialPort::Data8);
              break;
          default: break;
          }
          /* 设置奇偶校验 */
          switch (ui->check_bit->currentIndex()) {
          case 0:
              serialPort->setParity(QSerialPort::NoParity);
              break;
          case 1:
              serialPort->setParity(QSerialPort::EvenParity);
              break;
          case 2:
              serialPort->setParity(QSerialPort::OddParity);
              break;
          case 3:
              serialPort->setParity(QSerialPort::SpaceParity);
              break;
          case 4:
              serialPort->setParity(QSerialPort::MarkParity);
              break;
          default: break;
          }
          /* 设置停止位 */
          switch (ui->stop_bit->currentText().toInt()) {
          case 1:
              serialPort->setStopBits(QSerialPort::OneStop);
              break;
          case 2:
              serialPort->setStopBits(QSerialPort::TwoStop);
              break;
          default: break;
          }
          /* 设置流控制 */
          serialPort->setFlowControl(QSerialPort::NoFlowControl);
          if (!serialPort->open(QIODevice::ReadWrite))
              QMessageBox::warning(NULL, "error",
                                 "The serial port cannot be opened! \r\n"
                                 "Maybe the serial port is already occupied!");
          else {
              ui->port->setEnabled(false);
              ui->rate->setEnabled(false);
              ui->check_bit->setEnabled(false);
              ui->stop_bit->setEnabled(false);
              ui->data_bit->setEnabled(false);
              ui->open->setText("Close");
              ui->send->setEnabled(true);
          }
      } else {
          serialPort->close();
          ui->port->setEnabled(true);
          ui->rate->setEnabled(true);
          ui->check_bit->setEnabled(true);
          ui->stop_bit->setEnabled(true);
          ui->data_bit->setEnabled(true);
          ui->open->setText("Open");
          ui->send->setEnabled(false);
      }
}

void uart::serialPortReadyRead()
{
    /* 接收缓冲区中读取数据 */
    QByteArray buf = serialPort->readAll();

    ui->rx_browser->moveCursor(QTextCursor::End);
    ui->rx_browser->insertPlainText(QString(buf));
    ui->rx_browser->moveCursor(QTextCursor::End);

    qDebug() << buf << endl;
}

void uart::on_send_clicked()
{
    printf("on_send_clicked\r\n");
    /* 获取textEdit数据,转换成utf8格式的字节流 */
    QByteArray data = ui->tx_edit->toPlainText().toUtf8();
    serialPort->write(data);
    qDebug() << data << endl;
}

void uart::on_clear_clicked()
{
    printf("on_clear_clicked\r\n");
    ui->rx_browser->clear();
}
