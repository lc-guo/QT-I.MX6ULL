#ifndef SKETCHPAD_H
#define SKETCHPAD_H

#include <QMainWindow>
#include <QDir>

namespace Ui { class sketchpad; }

class sketchpad : public QMainWindow
{
    Q_OBJECT
public:
    explicit sketchpad(QWidget *parent = nullptr);
    ~sketchpad();

signals:

private slots:
    void on_sketchpad_back_clicked();
    void on_brush_clicked(bool checked);
    void on_eraser_clicked(bool checked);
    void on_save_clicked();
    void on_images_clicked();
    void on_colors_clicked();
    void on_clear_clicked();

private:
    Ui::sketchpad *ui;
    bool drawingEnabled = false;
    bool erasingEnabled = false;
    QPoint lastPoint;
    QImage draw_image;
    QDir save_dir;
    QBrush draw_brush = Qt::black;
    qreal draw_width = 5;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);
    void draw_timer_timeout();
    bool saveImage();
    bool isNotEmpty(const QImage &image);

};

#endif // SKETCHPAD_H
