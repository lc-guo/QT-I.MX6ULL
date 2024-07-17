#ifndef GPIO_H
#define GPIO_H

#include <QMainWindow>
#include <QFile>

namespace Ui { class gpio; }

class gpio : public QMainWindow
{
    Q_OBJECT
public:
    explicit gpio(QWidget *parent = nullptr);
    ~gpio();
signals:

private slots:
    void on_gpio_back_clicked();
    void on_beep_clicked(bool checked);
    void on_led_clicked(bool checked);


private:
    Ui::gpio *ui;
    QFile led_file;
    QFile beep_file;

    /* 设置lED的状态 */
    void setGpioState(QFile &file, bool value);
    /* 获取lED的状态 */
    bool getGpioState(QFile &file);

    /* 重写按键按下和释放事件 */
    void keyReleaseEvent(QKeyEvent *event);
    void keyPressEvent(QKeyEvent *event);

};


#endif // GPIO_H
