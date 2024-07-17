#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "camera.h"
#include "ap3216c.h"
#include "icm20608.h"
#include "uart.h"
#include "music_player.h"
#include "sketchpad.h"
#include "adc.h"
#include "gpio.h"
#include "can.h"
#include "ble.h"
#include "config.h"
#include "video.h"
#include "recording.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private:
    Ui::MainWindow *ui;
    // QScrollArea *pScroll;
    QPoint lastPos;
    QTimer *pressTimer;
    QPushButton *currentButton;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void connectButton(QPushButton *button);
    void handleLongPress();
};
#endif // MAINWINDOW_H
