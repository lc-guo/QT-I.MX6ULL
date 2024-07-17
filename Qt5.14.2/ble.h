#ifndef BLE_H
#define BLE_H

#include <QMainWindow>

namespace Ui { class ble; }

class ble : public QMainWindow
{
    Q_OBJECT
public:
    explicit ble(QWidget *parent = nullptr);
    ~ble();

signals:


private slots:
    void on_ble_back_clicked();

private:
    Ui::ble *ui;
};

#endif // BLE_H
