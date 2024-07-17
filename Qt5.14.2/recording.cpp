#include "recording.h"
#include "mainwindow.h"
#include "ui_recording.h"


recording::recording(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::recording)
{
    ui->setupUi(this);
    this->setWindowTitle("Recording");
    this->setFixedSize(800,480);


}

recording::~recording()
{
    delete ui;
}




void recording::on_recording_back_clicked()
{
    printf("on_recording_back_clicked\r\n");

    this->close();
    MainWindow *m = new MainWindow();
    m->show();
}
