#include "mainwindow.h"
#include "ui_mainwindow.h"

extern int scrollBarValue;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("I.MX6ULL");
    this->setFixedSize(1130,480);

    ui->scrollArea->setWidget(ui->widget);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setGeometry(0, 0, 800, 480);

    ui->widget->setMinimumSize(width(), height());

    pressTimer = new QTimer();
    pressTimer->setInterval(400);
    pressTimer->setSingleShot(true);
    connect(pressTimer, &QTimer::timeout, this, &MainWindow::handleLongPress);

    connectButton(ui->camera);
    connectButton(ui->sketchpad);
    connectButton(ui->adc);
    connectButton(ui->ble);
    connectButton(ui->can);
    connectButton(ui->gpio);
    connectButton(ui->uart);
    connectButton(ui->ap3216c);
    connectButton(ui->icm20608);
    connectButton(ui->music_player);
    connectButton(ui->video);
    connectButton(ui->config);
    connectButton(ui->recording);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event){
        lastPos = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    int dx = event->pos().x() - lastPos.x();
    ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->value() - dx / 2);
    lastPos = event->pos();
}

void MainWindow::handleLongPress() {
    printf("%s btn was long pressed\r\n",
           currentButton->text().toStdString().c_str());

    if (currentButton == ui->camera) {
        this->close();
        camera *c = new camera();
        c->show();
    } else if (currentButton == ui->sketchpad){
        this->close();
        sketchpad *c = new sketchpad();
        c->show();
    } else if (currentButton == ui->ap3216c){
        this->close();
        ap3216c *c = new ap3216c();
        c->show();
    } else if (currentButton == ui->icm20608){
        this->close();
        icm20608 *c = new icm20608();
        c->show();
    } else if (currentButton == ui->uart){
        this->close();
        uart *c = new uart();
        c->show();
    } else if (currentButton == ui->music_player){
        this->close();
        music_player *c = new music_player();
        c->show();
    } else if (currentButton == ui->gpio){
        this->close();
        gpio *c = new gpio();
        c->show();
    } else if (currentButton == ui->can){
        this->close();
        can *c = new can();
        c->show();
    } else if (currentButton == ui->ble){
        this->close();
        ble *c = new ble();
        c->show();
    } else if (currentButton == ui->adc){
        this->close();
        adc *c = new adc();
        c->show();
    } else if (currentButton == ui->video){
        this->close();
        video *c = new video();
        c->show();
    } else if (currentButton == ui->config){
        this->close();
        config *c = new config();
        c->show();
    } else if (currentButton == ui->recording){
        this->close();
        recording *c = new recording();
        c->show();
    }
}

void MainWindow::connectButton(QPushButton *button) {
    connect(button, &QPushButton::pressed, this, [this, button]() {
        currentButton = button;
        pressTimer->start();
    });

    connect(button, &QPushButton::released, this, [this, button]() {
        Q_UNUSED(button);
        if (pressTimer->isActive()) {
            pressTimer->stop();
        }
        currentButton = nullptr;
    });
}


void MainWindow::closeEvent(QCloseEvent *event) {
    scrollBarValue = ui->scrollArea->horizontalScrollBar()->value();
    QMainWindow::closeEvent(event);
}


void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);

    ui->scrollArea->hide();

    // 使用 QTimer::singleShot 延迟设置滚动条的值
    QTimer::singleShot(0, this, [this]() {
        ui->scrollArea->horizontalScrollBar()->setValue(scrollBarValue);
        ui->scrollArea->show();
    });
}
