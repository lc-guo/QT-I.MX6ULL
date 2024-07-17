#include "sketchpad.h"
#include "ui_sketchpad.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QDebug>

sketchpad::sketchpad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::sketchpad)
{
    ui->setupUi(this);
    this->setWindowTitle("Sketchpad");
    this->setFixedSize(800,480);
    draw_image = QImage(800, 480, QImage::Format_RGB32);
    draw_image.fill(Qt::white);

}


sketchpad::~sketchpad()
{
    delete ui;
}


void sketchpad::mousePressEvent(QMouseEvent *event){
    if((drawingEnabled || erasingEnabled) && (event->buttons() & Qt::LeftButton)){
        printf("mousePressEvent\r\n");
        lastPoint = event->pos();
    }
}

void sketchpad::mouseMoveEvent(QMouseEvent *event){
    if((drawingEnabled || erasingEnabled) && (event->buttons() & Qt::LeftButton)){
        printf("mouseMoveEvent\r\n");
        QPainter painter(&draw_image);
        if(drawingEnabled){
            painter.setPen(QPen(draw_brush,draw_width,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        } else if(erasingEnabled){
            painter.setPen(QPen(Qt::white,draw_width+5,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        }
        painter.drawLine(lastPoint, event->pos());
        lastPoint = event->pos();
        update();
    }
}

void sketchpad::wheelEvent(QWheelEvent *event){
    Q_UNUSED(event);
    if((drawingEnabled || erasingEnabled)){
        printf("wheelEvent\r\n");
        int numDegrees = event->angleDelta().y() / 8;
        int numSteps = numDegrees / 15;
        draw_width = draw_width + numSteps;
        draw_width = draw_width>10?10:draw_width;
        draw_width = draw_width<1?1:draw_width;
    }
}

void sketchpad::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawImage(0, 0, draw_image);
}

bool sketchpad::saveImage()
{

#ifdef __arm__
    QString saveDir = "/qt/images";
#else
    QString saveDir = QDir::currentPath() + "/images"; // 设置保存路径
#endif

    if (!save_dir.exists(saveDir)) {
        save_dir.mkpath(saveDir);
        printf("Create a file path\r\n");
    } else {
        printf("The file path already exists\r\n");
    }

    QString savePath = saveDir + "/draw_image_" +
            QString::number(QRandomGenerator::global()->bounded(0,9999)) + ".jpg";

    if(draw_image.save(savePath)){
        return true;
    }
    else{
        return false;
    }
}


void sketchpad::on_sketchpad_back_clicked()
{
    printf("on_sketchpad_back_clicked\r\n");

    this->close();
    MainWindow *m = new MainWindow();
    m->show();
}


void sketchpad::on_brush_clicked(bool checked)
{
    printf("on_brush_clicked %d\r\n", checked);
    if(checked){
        drawingEnabled = true;
        erasingEnabled = false;
        ui->eraser->setChecked(false);
    }
    else
        drawingEnabled = false;
}

void sketchpad::on_eraser_clicked(bool checked)
{
    printf("on_eraser_clicked %d\r\n", checked);
    if(checked){
        erasingEnabled = true;
        drawingEnabled = false;
        ui->brush->setChecked(false);
    }
    else
        erasingEnabled = false;
}

void sketchpad::on_save_clicked()
{
    printf("on_save_clicked \r\n");

    if(isNotEmpty(draw_image)){
        if(saveImage()){
            QMessageBox::information(this, "Saved successfully", "Image saved successfully!");
        }
        else{
            QMessageBox::warning(this, "Save failed", "Image saving failed!");
        }
    } else {
        QMessageBox::warning(this, "Save failed", "Image is empty!");
    }
}

void sketchpad::on_images_clicked()
{
    printf("on_images_clicked\r\n");
    this->close();

    showphoto *s = new showphoto(2);
    s->show();
}

void sketchpad::on_colors_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Color");

    if (color.isValid()) {
        qDebug() << color.name() << endl;
        draw_brush.setColor(color);
    }
}

bool sketchpad::isNotEmpty(const QImage &image)
{
    // 获取图像的尺寸
    int width = image.width();
    int height = image.height();

    // 遍历图像的每个像素
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // 获取当前像素的颜色
            QColor color = image.pixelColor(x, y);

            // 检查颜色是否为白色
            if (color != QColor(Qt::white)) {
                return true;  // 如果找到不为白色的像素，则图像不为空
            }
        }
    }

    return false;  // 如果所有像素都为白色，则图像为空
}


void sketchpad::on_clear_clicked()
{
    printf("on_clear_clicked\r\n");
    draw_image.fill(Qt::white);
    update();
}
