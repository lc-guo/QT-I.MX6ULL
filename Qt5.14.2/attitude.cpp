#include "attitude.h"

Attitude::Attitude(QWidget *parent):QOpenGLWidget(parent)
{
    //setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
}

void Attitude::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
}

void Attitude::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Attitude::paintGL()
{
     // 清除屏幕和Z缓冲区
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    // 重置转换
    glLoadIdentity();

    // 当用户更改“ rotate_x”和“ rotate_y”时旋转
    glRotatef( rotate_x, 1.0, 0.0, 0.0 );
    glRotatef( rotate_y, 0.0, 1.0, 0.0 );
    glRotatef( rotate_z, 0.0, 0.0, 1.0 );

    // 前侧：多色侧
    glBegin(GL_POLYGON); // 显示一个矩形，就使用GL_POLYGON

    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  0.5, -0.5, -0.5 );      // P1 是红色
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f(  0.5,  0.5, -0.5 );      // P2 是绿色
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f( -0.5,  0.5, -0.5 );      // P3 是蓝色
    glColor3f( 1.0, 0.0, 1.0 );
    glVertex3f( -0.5, -0.5, -0.5 );      // P4 是紫色

    glEnd();

     // 背面：白色面
    glBegin(GL_POLYGON);
    glColor3f(   1.0,  1.0, 1.0 );
    glVertex3f(  0.5, -0.5, 0.5 );
    glVertex3f(  0.5,  0.5, 0.5 );
    glVertex3f( -0.5,  0.5, 0.5 );
    glVertex3f( -0.5, -0.5, 0.5 );
    glEnd();

    // 右侧：紫色侧
    glBegin(GL_POLYGON);
    glColor3f(  1.0,  0.0,  1.0 );
    glVertex3f( 0.5, -0.5, -0.5 );
    glVertex3f( 0.5,  0.5, -0.5 );
    glVertex3f( 0.5,  0.5,  0.5 );
    glVertex3f( 0.5, -0.5,  0.5 );
    glEnd();

    // 左侧：绿色侧
    glBegin(GL_POLYGON);
    glColor3f(   0.0,  1.0,  0.0 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();

    // 顶部：蓝色面
    glBegin(GL_POLYGON);
    glColor3f(   0.0,  0.0,  1.0 );
    glVertex3f(  0.5,  0.5,  0.5 );
    glVertex3f(  0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glEnd();

    // 下侧：红色侧
    glBegin(GL_POLYGON);
    glColor3f(   1.0,  0.0,  0.0 );
    glVertex3f(  0.5, -0.5, -0.5 );
    glVertex3f(  0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();

    graficarLineas();
    glFlush();
    this->makeCurrent();
}

void Attitude::graficarLineas()
{
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(20,0,0);

    glColor3f(1,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,20,0);

    glColor3f(0,1,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,20);
    glEnd();
}

void Attitude::SetRotate(double x,double y,double z)
{
    rotate_x=x;
    rotate_y=y;
    rotate_z=z;
    update();
}
