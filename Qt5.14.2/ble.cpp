#include "ble.h"
#include "mainwindow.h"
#include "ui_ble.h"

ble::ble(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ble)
{
    ui->setupUi(this);
    this->setWindowTitle("BLE");
    this->setFixedSize(800,480);
}


ble::~ble()
{
    delete ui;
}

void ble::on_ble_back_clicked()
{
    printf("on_ble_back_clicked\r\n");
    this->close();

    MainWindow *m = new MainWindow();
    m->show();
}
