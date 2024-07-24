#include "can.h"
#include "ui_can.h"
#include "mainwindow.h"

can::can(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::can)
{
    ui->setupUi(this);
    this->setWindowTitle("CAN");
    this->setFixedSize(800,480);

    ui->rx_browser->setPlaceholderText("Time ID Length Data");
    ui->tx_edit->setText("123 aa 77 66 55");
    ui->rx_browser->show();
    ui->tx_edit->show();

    ui->send->setEnabled(false);
    ui->label_message->setText(tr("not connected!"));

    //canDevice = new QCanBusDevice(this);

#ifdef __arm__
    system("ifconfig can0 down");
    // 自动重启延迟为 100 毫秒
    system("ip link set up can0 type can bitrate 1000000 restart-ms 100");
#endif

    pluginItemInit();
    bandRateItemInit();
}


can::~can()
{
    canDevice->disconnectDevice();
    delete canDevice;
    delete ui;
}

/* 从系统中读取可用的插件，并显示到comboBox[0] */
void can::pluginItemInit()
{
    ui->plugin->addItems(QCanBus::instance()->plugins());
    for (int i = 0; i < QCanBus::instance()->plugins().count(); i++) {
        if (QCanBus::instance()->plugins().at(i) == "socketcan")
            ui->plugin->setCurrentIndex(i);
    }
}


/* 初始化一些常用的比特率，can的比特率不是随便设置的，有相应的计算公式 */
void can::bandRateItemInit()
{
    const QList<int> rates = {
        10000, 20000, 50000, 125000, 156250,
        250000, 500000, 700000 , 800000, 1000000
    };

    for (int rate : rates)
        ui->rate->addItem(QString::number(rate), rate);

    /* 默认初始化以1000000比特率 */
    ui->rate->setCurrentIndex(7);
}


static QString frameFlags(const QCanBusFrame &frame)
{
    /* 格式化接收到的消息 */
    QString result = QLatin1String(" --- ");

    if (frame.hasBitrateSwitch())
        result[1] = QLatin1Char('B');
    if (frame.hasErrorStateIndicator())
        result[2] = QLatin1Char('E');
    if (frame.hasLocalEcho())
        result[3] = QLatin1Char('L');

    return result;
}


/* 接收消息 */
void can::receivedFrames()
{
    if (!canDevice)
        return;

    /* 读取帧 */
    while (canDevice->framesAvailable()) {
        const QCanBusFrame frame = canDevice->readFrame();
        QString view;
        if (frame.frameType() == QCanBusFrame::ErrorFrame)
            view = canDevice->interpretErrorFrame(frame);
        else
            view = frame.toString();

        const QString time = QString::fromLatin1("%1.%2  ")
                .arg(frame.timeStamp()
                     .seconds(), 10, 10, QLatin1Char(' '))
                .arg(frame.timeStamp()
                     .microSeconds() / 100, 4, 10, QLatin1Char('0'));

        const QString flags = frameFlags(frame);
        /* 接收消息框追加接收到的消息 */
        ui->rx_browser->moveCursor(QTextCursor::End);
        ui->rx_browser->insertPlainText(time + flags + view + "\n");
        ui->rx_browser->moveCursor(QTextCursor::End);
        qDebug() << time + flags + view << endl;

    }
}

void can::on_can_back_clicked()
{
    printf("on_can_back_clicked\r\n");
    QString cmd1 = tr("ifconfig %1 down")
            .arg(ui->port->currentText());

    this->close();
    MainWindow *m = new MainWindow();
    m->show();
}

void can::on_clear_clicked()
{
    printf("on_clear_clicked\r\n");
    ui->rx_browser->clear();
}

void can::on_connect_clicked()
{
    printf("on_connect_clicked\r\n");
#ifdef __arm__
    if (ui->connect->text() == "Connect") {
      /* Qt中的QCanBusDevice::BitRateKey不能设置比特率 */
      QString cmd1 = tr("ifconfig %1 down")
              .arg(ui->port->currentText());
      QString cmd2 =
              tr("ip link set up %1 type can bitrate %2 restart-ms 100")
              .arg(ui->port->currentText())
              .arg(ui->rate->currentText());
      /* 使用系统指令以设置的比特率初始化CAN */
      system(cmd1.toStdString().c_str());
      system(cmd2.toStdString().c_str());

      QString errorString;
      /* 以设置的插件名与接口实例化 canDevice */
      canDevice = QCanBus::instance()->
              createDevice(ui->plugin->currentText(),
              ui->port->currentText(),
              &errorString);

      if (!canDevice) {
          ui->label_message->setText(
                      tr("Error creating device '%1', reason: '%2'")
                      .arg(ui->plugin->currentText())
                  .arg(errorString));
          return;
      }

      /* 连接CAN */
      if (!canDevice->connectDevice()) {
          ui->label_message->setText(tr("Connection error: %1")
                            .arg(canDevice->errorString()));
          delete canDevice;
          canDevice = nullptr;

          return;
      }

      /* 处理接收到的消息 */
      connect(canDevice, SIGNAL(framesReceived()),
              this, SLOT(receivedFrames()));
      connect(canDevice,
              SIGNAL(errorOccurred(QCanBusDevice::CanBusError)),
              this,
              SLOT(canDeviceErrors(QCanBusDevice::CanBusError)));

      /* 将连接信息插入到label */
      ui->label_message->setText(
                  tr("plugin: %1, connected %2, band rate: %3 kBit/s")
                  .arg(ui->plugin->currentText())
              .arg(ui->port->currentText())
              .arg(ui->rate->currentText().toInt() / 1000));
      ui->connect->setText("Disconnect");

      /* 使能/失能 */
      ui->send->setEnabled(true);
      ui->plugin->setEnabled(false);
      ui->port->setEnabled(false);
      ui->rate->setEnabled(false);
  } else {
      if (!canDevice)
          return;

      /* 断开连接 */
      canDevice->disconnectDevice();
      delete canDevice;
      canDevice = nullptr;
      ui->connect->setText("Connect");
      ui->send->setEnabled(false);
      ui->label_message->setText(tr("not connected!"));
      ui->plugin->setEnabled(true);
      ui->port->setEnabled(true);
      ui->rate->setEnabled(true);
  }
#endif
}

void can::on_send_clicked()
{
    printf("on_send_clicked\r\n");
    if (!canDevice){
        printf("no find canDevice\r\n");
       return;
    }
#ifdef __arm__
    /* 读取QLineEdit的文件 */
    QString str = ui->tx_edit->text();
    QByteArray data = 0;
    QString strTemp = nullptr;

    /* 以空格分隔lineEdit的内容，并存储到字符串链表中 */
    QStringList strlist = str.split(' ');
    for (int i = 1; i < strlist.count(); i++) {
       strTemp = strTemp + strlist[i];
    }
    /* 将字符串的内容转为QByteArray类型 */
    data = QByteArray::fromHex(strTemp.toLatin1());

    bool ok;
    /* 以16进制读取要发送的帧内容里第一个数据，并作为帧ID */
    int framId = strlist[0].toInt(&ok, 16);
    QCanBusFrame frame = QCanBusFrame(framId, data);
    /* 写入帧 */
    canDevice->writeFrame(frame);
#endif
}

void can::on_plugin_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QList<QCanBusDeviceInfo> interfaces;
    ui->port->clear();
    /* 当我们改变插件时，我们同时需要将可用接口，从插件类型中读取出来 */
    interfaces = QCanBus::instance()
            ->availableDevices(ui->plugin->currentText());
    for (const QCanBusDeviceInfo &info : qAsConst(interfaces)) {
        ui->port->addItem(info.name());
    }
}

void can::canDeviceErrors(QCanBusDevice::CanBusError error) const
{
    /* 错误处理 */
    switch (error) {
    case QCanBusDevice::ReadError:
    case QCanBusDevice::WriteError:
    case QCanBusDevice::ConnectionError:
    case QCanBusDevice::ConfigurationError:
    case QCanBusDevice::UnknownError:
        ui->label_message->setText(canDevice->errorString());
        break;
    default:
        break;
    }
}
