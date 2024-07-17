#ifndef ICM20608_H
#define ICM20608_H

#include <QMainWindow>
#include <QTimer>

namespace Ui { class icm20608; }

#define gyroscale (2000.0 / 32767.0 * 0.0174532925)
#define accscale  (16.0 / 32767.0)
#define NUM_SAMPLES 1000
#define Kp      40.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki      0.008f                       // integral gain governs rate of convergence of gyroscope biases
#define halfT   0.001f                       // half the sample period


class icm20608 : public QMainWindow
{
    Q_OBJECT
public:
    explicit icm20608(QWidget *parent = nullptr);
    ~icm20608();

    float yaw = 0;
    float pitch = 0;
    float roll = 0;

    void calibrate_gyroscope(int16_t *gyro_bias);
    void icm20608_get_six_axis_data(float *gyro, float *acc);
    float icm20608_get_temperature();
    void icm20608_update(float gx, float gy, float gz, float ax, float ay, float az);
    void update_attitude_3D();


private:
    Ui::icm20608 *ui;
    QTimer *timer;
    int fd;
    char const *filename = "/dev/icm20608";
    signed int databuf[7];
    int16_t gyro_bias[3];

    float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
    float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error

    float gyro[3], acc[3], temp;


signals:

private slots:
    void on_icm20608_back_clicked();


public slots:
    void icm20608_timer_timeout();

};

#endif // ICM20608_H
