#include "showphoto.h"
#include "ui_showphoto.h"
#include "mainwindow.h"

showphoto::showphoto(int sourcePage, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::showphoto)
{
    printf("showphoto ui run\r\n");
    ui->setupUi(this);
    this->setWindowTitle("Photo");
    this->setFixedSize(800,480);

    srcPage = sourcePage;

    // 相册
    if(sourcePage == 1){
#ifdef __arm__
        dir.setPath("/qt/photo/");
#else
        dir.setPath("./photo/"); // 设置目录路径
#endif
    }
    // 画图板
    else if(sourcePage == 2){
#ifdef __arm__
        dir.setPath("/qt/images/");
#else
        dir.setPath("./images/"); // 设置目录路径
#endif
    }
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot); // 过滤条件，只获取文件
    dir.setNameFilters(QStringList() << "*.jpg"); // 文件名过滤，只获取jpg文件
    dir.setSorting(QDir::Time | QDir::Reversed);  // 根据修改时间进行排序，逆序排列
    fileList = dir.entryList(); // 获取文件列表

    //先显示一张图片
    if(fileList.length() > 0){
        QString filePath = dir.filePath(fileList[currentIndex]); // 获取文件路径
        QPixmap pixmap(filePath); // 创建图片对象
        ui->label->setPixmap(pixmap); // 在控件上显示图片
    }

   // 将QLabel嵌入到QScrollArea中
   ui->scrollArea->setWidgetResizable(true);
   ui->scrollArea->setGeometry(0, 0, 800, 480);

#ifdef __arm__
  // 设置pressTimer的超时和槽函数
  pressTimer = new QTimer(this);
  pressTimer->setSingleShot(true);
  pressTimer->setInterval(500); // 长按时间阈值（500毫秒）
  connect(pressTimer, &QTimer::timeout, this, &showphoto::onLongPress);
#endif
}



showphoto::~showphoto()
{
    delete ui;
}

void showphoto::on_showphoto_next_clicked()
{
    if(fileList.length() > 0){
        currentIndex++; // 更新当前显示的文件索引
        if (currentIndex >= fileList.size()) {
            currentIndex = 0;
        }

        if(isZoomed){
            printf("Force restore view size\r\n");
#ifdef __arm__
            onLongPress();
#else
            QMouseEvent *dblClickEvent = new QMouseEvent(QEvent::MouseButtonDblClick,
                                                            ui->label->rect().center(),
                                                            Qt::LeftButton,
                                                            Qt::LeftButton,
                                                            Qt::NoModifier);

            // 发送事件到目标对象
            QCoreApplication::postEvent(ui->label, dblClickEvent);
#endif
        }

        QString filePath = dir.filePath(fileList[currentIndex]); // 获取文件路径
        QPixmap pixmap(filePath); // 创建图片对象
        ui->label->setPixmap(pixmap); // 在控件上显示图片
    }
}

void showphoto::on_showphoto_back_clicked()
{
    printf("on_showphoto_back_clicked\r\n");
    this->close();
    if(srcPage == 1){
        camera *v = new camera();
        v->show();
    } else if(srcPage == 2){
        sketchpad *s = new sketchpad();
        s->show();
    }
}

void showphoto::on_showphoto_front_clicked()
{
    if(fileList.length() > 0){
        currentIndex--; // 更新当前显示的文件索引
        if (currentIndex < 0) {
            currentIndex = fileList.size()-1;
        }

        if(isZoomed){
            printf("Force restore view size\r\n");
#ifdef __arm__
            onLongPress();
#else
            QMouseEvent *dblClickEvent = new QMouseEvent(QEvent::MouseButtonDblClick,
                                                            ui->label->rect().center(),
                                                            Qt::LeftButton,
                                                            Qt::LeftButton,
                                                            Qt::NoModifier);

            // 发送事件到目标对象
            QCoreApplication::postEvent(ui->label, dblClickEvent);
#endif
        }

        QString filePath = dir.filePath(fileList[currentIndex]); // 获取文件路径
        QPixmap pixmap(filePath); // 创建图片对象
        ui->label->setPixmap(pixmap); // 在控件上显示图片
    }
}
#ifdef __arm__
void showphoto::onLongPress()
#else
void showphoto::mouseDoubleClickEvent(QMouseEvent *event)
#endif
{
    if (fileList.length() > 0) {
        QString filePath = dir.filePath(fileList[currentIndex]); // 获取文件路径
        QPixmap pixmap(filePath); // 创建图片对象

        if (isZoomed) {
           // 恢复原来的大小
           ui->label->setPixmap(pixmap);
           ui->label->move((ui->scrollArea->width() - ui->label->width()) / 2,
                           (ui->scrollArea->height() - ui->label->height()) / 2);
           ui->scrollArea->setWidgetResizable(true);
           isZoomed = false;
        } else {
            // 放大显示
            QPixmap scaledPixmap = pixmap.scaled(pixmap.size() * 2, Qt::KeepAspectRatio);
            ui->label->setPixmap(scaledPixmap);
            ui->label->resize(scaledPixmap.size());
            ui->scrollArea->setWidgetResizable(false);
            isZoomed = true;
        }
    }
#ifndef __arm__
    QMainWindow::mouseDoubleClickEvent(event); // 保持默认行为
#endif
}


void showphoto::mousePressEvent(QMouseEvent *event)
{
    if (isZoomed) {
        dragging = true;
        lastMousePos = event->pos();
    }
#ifdef __arm__
    pressTimer->start();
#endif
    QMainWindow::mousePressEvent(event); // 保持默认行为
}

void showphoto::mouseMoveEvent(QMouseEvent *event)
{
    if (dragging && isZoomed) {
        int dx = event->pos().x() - lastMousePos.x();
        int dy = event->pos().y() - lastMousePos.y();

        ui->label->move(ui->label->x() + dx, ui->label->y() + dy);
        lastMousePos = event->pos();
    }
#ifdef __arm__
    pressTimer->stop();
#endif
    QMainWindow::mouseMoveEvent(event); // 保持默认行为
}

void showphoto::mouseReleaseEvent(QMouseEvent *event)
{
    if (isZoomed) {
        dragging = false;
    }
#ifdef __arm__
    pressTimer->stop();
#endif
    QMainWindow::mouseReleaseEvent(event); // 保持默认行为
}
