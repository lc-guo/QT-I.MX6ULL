#ifndef CAN_H
#define CAN_H

#include <QMainWindow>
#include <QCanBusDevice>
#include <QCanBus>

namespace Ui { class can; }


class can : public QMainWindow
{
    Q_OBJECT
public:
    explicit can(QWidget *parent = nullptr);
    ~can();
signals:

private slots:
    void on_can_back_clicked();
    void on_clear_clicked();
    void on_connect_clicked();
    void on_send_clicked();
    void receivedFrames();

    void on_plugin_currentIndexChanged(int index);

private:
    Ui::can *ui;
    bool isConnected = false;
    /* CAN设备 */
    QCanBusDevice *canDevice;
    void bandRateItemInit();
    void pluginItemInit();

    void canDeviceErrors(QCanBusDevice::CanBusError error) const;



};

#endif // CAN_H
