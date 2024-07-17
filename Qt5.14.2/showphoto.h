#ifndef SHOWPHOTO_H
#define SHOWPHOTO_H

#include <QMainWindow>
#include <QDir>
#include <QDebug>
#include <QScrollArea>
#define printf qDebug

namespace Ui { class showphoto; }

class showphoto : public QMainWindow
{
    Q_OBJECT
public:
    explicit showphoto(int sourcePage, QWidget *parent = nullptr);
    ~showphoto();


signals:

private slots:
    void on_showphoto_next_clicked();
    void on_showphoto_back_clicked();
    void on_showphoto_front_clicked();

private:
    Ui::showphoto *ui;
    int srcPage;
    QDir dir; // 目录对象
    QStringList fileList; // 存储文件列表
    int currentIndex = 0; // 当前显示的文件索引
    bool isZoomed = false;
    bool dragging  = false;
    QPoint lastMousePos;
    QTimer *pressTimer;

#ifdef __arm__
void onLongPress();
#else
    void mouseDoubleClickEvent(QMouseEvent *event);
#endif
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


};

#endif // SHOWPHOTO_H
