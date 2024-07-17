#ifndef CONFIG_H
#define CONFIG_H

#include <QMainWindow>
#include <QFile>

namespace Ui { class config; }

class config : public QMainWindow
{
    Q_OBJECT
public:
    explicit config(QWidget *parent = nullptr);
    ~config();

signals:


private slots:
    void on_config_back_clicked();
    void on_brightness_slider_valueChanged(int value);

private:
    Ui::config *ui;
    QFile brightness_file;
    void setBrightness(QFile &file, int value);
    void showEvent(QShowEvent *event);


};

#endif // CONFIG_H
