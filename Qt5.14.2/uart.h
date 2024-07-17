#ifndef UART_H
#define UART_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui { class uart; }

class uart : public QMainWindow
{
    Q_OBJECT
public:
    explicit uart(QWidget *parent = nullptr);
    ~uart();

private:
    Ui::uart *ui;
    QSerialPort *serialPort;


    void scanSerialPort();
    void baudRateItemInit();
    void dataBitsItemInit();
    void parityItemInit();
    void stopBitsItemInit();


signals:

private slots:
    void on_uart_back_clicked();
    void on_open_clicked();
    void on_send_clicked();
    void serialPortReadyRead();
    void on_clear_clicked();
};

#endif // UART_H
