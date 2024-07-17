#include "mainwindow.h"
#include "icm20608.h"
#include "ui_icm20608.h"

icm20608::icm20608(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::icm20608)
{
    ui->setupUi(this);
    this->setWindowTitle("Icm20608");
    this->setFixedSize(800,480);

    timer = new QTimer();
#if __arm__
    system("cd /lib/modules/4.1.15");
    system("depmod");
    system("modprobe icm20608.ko");
#endif

    connect(timer, SIGNAL(timeout()), this, SLOT(icm20608_timer_timeout()));

#if __arm__
    fd = open(filename, O_RDWR);
    if(fd < 0) {
        printf("can't open file %s\r\n", filename);
        return;
    }

    calibrate_gyroscope(gyro_bias);

#endif

    timer->start(10);
}


icm20608::~icm20608()
{
    delete ui;
    delete timer;
}



void icm20608::calibrate_gyroscope(int16_t *gyro_bias)
{
    int32_t gyro_sum[3] = {0, 0, 0};
    int16_t gyro_raw[3];
    int ret = 0;
    unsigned int i;

    for (i = 0; i < NUM_SAMPLES; i++)
    {
        ret = read(fd, databuf, sizeof(databuf));
        if(ret == 0) {
            gyro_raw[0] = databuf[0];
            gyro_raw[1] = databuf[1];
            gyro_raw[2] = databuf[2];
            gyro_sum[0] += gyro_raw[0];
            gyro_sum[1] += gyro_raw[1];
            gyro_sum[2] += gyro_raw[2];
        }
    }

    gyro_bias[0] = gyro_sum[0] / NUM_SAMPLES;
    gyro_bias[1] = gyro_sum[1] / NUM_SAMPLES;
    gyro_bias[2] = gyro_sum[2] / NUM_SAMPLES;

    printf("Calibrate_Gyroscope done!\r\n");
}



void icm20608::icm20608_get_six_axis_data(float *gyro, float *acc)
{
    unsigned char i = 0;
    int ret = 0;
    short gyro_raw[3], acc_raw[3];

    ret = read(fd, databuf, sizeof(databuf));
    if(ret == 0) {
        gyro_raw[0] = databuf[0];
        gyro_raw[1] = databuf[1];
        gyro_raw[2] = databuf[2];
        acc_raw[0] = databuf[3];
        acc_raw[1] = databuf[4];
        acc_raw[2] = databuf[5];
    }


    for(i=0;i<3;i++){
        gyro[i]= (float) (gyro_raw[i]-gyro_bias[i]) * gyroscale;
        acc[i]= (float) acc_raw[i] * accscale;
        //acc[i] = acc_calib_k[i] * acc[i] + acc_calib_a[i];
    }
}



float icm20608::icm20608_get_temperature()
{
    short raw = 0;
    float temp;
    int ret = 0;

    ret = read(fd, databuf, sizeof(databuf));
    if(ret == 0) {
        raw = databuf[6];
    }

    temp = 21 + ((double)raw) / 333.87;

    return temp;;
}



void icm20608::icm20608_update(float gx, float gy, float gz, float ax, float ay, float az)
{
    float norm;
    float vx, vy, vz;
    float ex, ey, ez;

    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    //float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    //float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    if (ax * ay * az == 0)
    {
        return;
    }

    norm = sqrt(ax * ax + ay * ay + az * az);       //
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    // estimated direction of gravity and flux (v and w)
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3 ;

    // error is sum of cross product between reference direction of fields and direction measured by sensors
    ex = (ay * vz - az * vy) ;
    ey = (az * vx - ax * vz) ;
    ez = (ax * vy - ay * vx) ;

    exInt = exInt + ex * Ki;
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

    // adjusted gyroscope measurements
    gx = gx + Kp * ex + exInt;
    gy = gy + Kp * ey + eyInt;
    gz = gz + Kp * ez + ezInt;

    // integrate quaternion rate and normalise
    q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
    q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
    q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
    q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;

    // normalise quaternion
    norm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.3; // unit:degree
    pitch  = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3; // unit:degree
    roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3; // unit:degree
}


void icm20608::update_attitude_3D()
{
    ui->ADI->setRoll(pitch);
    ui->ADI->setPitch(roll);
    ui->ADI->update();
    ui->label_x->setNum(yaw);
    ui->label_y->setNum(pitch);
    ui->label_z->setNum(roll);
}

void icm20608::icm20608_timer_timeout()
{
#ifdef __arm__
    icm20608_get_six_axis_data(gyro, acc);
    icm20608_update(gyro[0], gyro[1], gyro[2], acc[0], acc[1], acc[2]);
#else
    pitch ++;
    roll ++;
    yaw ++;
    pitch = pitch > 180?(-180):pitch;
    roll = roll > 180?(-180):roll;
    yaw = yaw > 180?(-180):yaw;

//    pitch = QRandomGenerator::global()->bounded(-5,5);
//    roll = QRandomGenerator::global()->bounded(-5,5);
//    yaw  = QRandomGenerator::global()->bounded(-5,5);
#endif
    update_attitude_3D();
}



void icm20608::on_icm20608_back_clicked()
{
    printf("on_icm20608_back_clicked\r\n");
    timer->stop();

#if __arm__
    system("rmmod icm20608.ko");
#endif

    this->close();
    MainWindow *m = new MainWindow();
    m->show();
}


