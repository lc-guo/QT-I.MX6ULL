#ifndef ATTITUDE_H
#define ATTITUDE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <QGLFramebufferObjectFormat>

class Attitude : public QOpenGLWidget, protected QOpenGLFunctions
{
public:

    explicit Attitude(QWidget *parent = 0);

    void SetRotate(double x,double y,double z);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h)Q_DECL_OVERRIDE;
    void paintGL()Q_DECL_OVERRIDE;
    void graficarLineas();

public:
    double rotate_y=0;
    double rotate_x=0;
    double rotate_z=0;

private:
};

#endif // Attitude_H

