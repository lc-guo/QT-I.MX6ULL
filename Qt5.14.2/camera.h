#ifndef CAMERA_H
#define CAMERA_H

#include <QMainWindow>
#include <QTimer>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <QRandomGenerator>
#include <unistd.h>             //write、close

#include "showphoto.h"

#define		video_width		800
#define 	video_height	480

namespace Ui { class camera; }

class camera : public QMainWindow
{
    Q_OBJECT
public:
    explicit camera(QWidget *parent = nullptr);
    ~camera();

private slots:
    void on_camera_open_clicked();
    void on_camera_photos_clicked();
    void on_camera_take_clicked();
    void on_camera_back_clicked();

private:
    Ui::camera *ui;
    QTimer *timer;
    int video_fd;
    int start = 0;

    char *userbuff[4];        /* 存放摄像头设备内核缓冲区映射后的用户内存地址 */
    int userbuff_length[4];   /* 保存映射后的数据长度，释放缓存时要用 */

    int camera_open(void);
    int camera_close(void);

public slots:
    void video_show(void);
};

#endif // V4L2_H
