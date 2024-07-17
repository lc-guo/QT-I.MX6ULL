#include "music_player.h"
#include "ui_music_player.h"
#include "mainwindow.h"

music_player::music_player(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::music_player)
{
    ui->setupUi(this);
    this->setWindowTitle("Music_Player");
    this->setFixedSize(800,480);

    mplayerProcess = new QProcess(this);

    // 初始化默认音乐路径
    defaultMusicPath = "/music";
    currentFilePath = "";
    currentIndex = -1;
    currentProgressValue = -1;
    currentProgressLabel = "";

    // 获取所有音乐文件
    loadMusicFiles();

    connect(mplayerProcess, &QProcess::readyReadStandardOutput, this, &music_player::handleMPlayerOutput);
    connect(mplayerProcess, &QProcess::readyReadStandardError, this, &music_player::handleMPlayerError);

    // 定时器定时读取 MPlayer 的播放状态
    progressTimer = new QTimer(this);
    connect(progressTimer, &QTimer::timeout, this, &music_player::updateProgress);
    progressTimer->start(900);

    // 初始化播放图标
    ui->start_pause->setIcon(QIcon(":/images/pause.svg"));
    ui->start_pause->setIconSize(QSize(48, 48));

    // 初始化黑胶 CD
    QPixmap originalCDImage(":/images/cd_image.png");
    cd_image = originalCDImage.scaled(ui->label_cd->width(),
                                     ui->label_cd->height(),
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);
    ui->label_cd->setPixmap(cd_image);
    ui->label_cd->setFixedSize(ui->label_cd->width(), ui->label_cd->height());

    // 黑胶转动定时器
    cd_timer = new QTimer(this);
    connect(cd_timer, &QTimer::timeout, this, &music_player::updateCD);
    cd_angle = 0;

    // 黑胶指针
    QPixmap originalPointerImage(":/images/cd_pointer.png");
    pointer_image = originalPointerImage.scaled(ui->label_cd->width(),
                                             ui->label_cd->height(),
                                             Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation);


    ui->label_pointer->setPixmap(pointer_image);
    ui->label_pointer->setFixedSize(ui->label_cd->width(), ui->label_cd->height());

    pointer_timer = new QTimer(this);
    connect(pointer_timer, &QTimer::timeout, this, &music_player::updatePointer);
    pointer_angle = 0;

    // 初始化播放模式
    currentMode = Sequential;
    ui->mode->setIcon(QIcon(":/images/sequential.svg"));
    ui->mode->setIconSize(QSize(32, 32));

    // 初始化音量播放器
    volume_timer = new QTimer(this);
    connect(volume_timer, &QTimer::timeout, this, [this]() {
            volume_timer->stop();
            ui->volume_widget->setVisible(false);
        });
    ui->volume_Slider->setValue(80);
    ui->volume_label->setText("80%");
    ui->volume_widget->setVisible(false);

    // 初始化音乐进度条
    ui->progress->setEnabled(false);
}

music_player::~music_player()
{
    delete mplayerProcess;
    delete progressTimer;
    delete ui;
}


void music_player::updateProgress()
{
    if (mplayerProcess->state() != QProcess::Running) {
        return;
    }

    if(play_flag == true){
        mplayerProcess->write("get_time_length\n");
        mplayerProcess->write("get_time_pos\n");
        mplayerProcess->write("get_percent_pos\n");
    }
}


void music_player::updateCD()
{
    if(cd_angle > 360){
        cd_angle=0;
    }

    cd_angle++;

    // 旋转缩放后的图像
    QTransform transform;
    int centerX = cd_image.width() / 2;
    int centerY = cd_image.height() / 2;

    // 将旋转中心移到图像中心
    transform.translate(centerX, centerY);
    // 执行旋转操作
    transform.rotate(cd_angle);
    // 将旋转中心移回原始位置
    transform.translate(-centerX, -centerY);

    QPixmap rotatedPixmap = cd_image.transformed(transform, Qt::SmoothTransformation);

    // 将旋转后的图像设置到 QLabel 上
    ui->label_cd->setPixmap(rotatedPixmap);
}

void music_player::updatePointer()
{
    pointer_angle = pointer_angle>50?50:pointer_angle;
    pointer_angle = pointer_angle<0?0:pointer_angle;

    if(play_flag){
        pointer_angle++;
    } else {
        pointer_angle--;
    }

    if(pointer_angle > 50 || pointer_angle < 0){
        pointer_timer->stop();
    }

    // 旋转缩放后的图像
    QTransform transform;
    int centerX = pointer_image.width() / 2;
    int centerY = pointer_image.height() / 2;

    // 将旋转中心移到图像中心
    transform.translate(centerX, centerY);
    // 执行旋转操作
    transform.rotate(pointer_angle);
    // 将旋转中心移回原始位置
    transform.translate(-centerX, -centerY);

    QPixmap rotatedPixmap = pointer_image.transformed(transform, Qt::SmoothTransformation);

    // 将旋转后的图像设置到 QLabel 上
    ui->label_pointer->setPixmap(rotatedPixmap);
}


void music_player::handleMPlayerOutput()
{
    while (mplayerProcess->canReadLine()) {
        QString message(mplayerProcess->readLine());

        QStringList message_list = message.split("=");
        // get_time_pos
        if(message_list[0] == "ANS_TIME_POSITION"){
            ui->progress->setValue(((float)currentPosition / (float)duration) * 100);

            currentPosition = message_list[1].toDouble();
            // 构造时间标签
            QString label_time = QString("%1%2:%3%4 / %5%6:%7%8")
                                .arg(currentPosition/60/10).arg(currentPosition/60%10)
                                .arg(currentPosition%60/10).arg(currentPosition%60%10)
                                .arg(duration/60/10).arg(duration/60%10)
                                .arg(duration%60/10).arg(duration%60%10);

             ui->label_progress->setText(label_time);
        }
        // get_time_length
        else if (message_list[0] == "ANS_LENGTH") {
            duration = message_list[1].toDouble();//返回的是秒为单位
        }
        // get_percent_pos
        else if (message_list[0] == "ANS_PERCENT_POSITION") { //返回当前歌曲的播放进度，单位是百分比 0-100
            currentProgressValue = message_list[1].toInt();
            //qDebug()<<value;
            ui->progress->setValue(currentProgressValue);
            if(currentProgressValue == 99){
                ui->start_pause->setIcon(QIcon(":/images/pause.svg"));
                on_next_clicked();
            }
        }
    }
}

void music_player::handleMPlayerError()
{
    QByteArray error = mplayerProcess->readAllStandardError();
    printf("MPlayer Error: %s\r\n", error.constData());
}

void music_player::loadMusicFiles()
{
    QStringList filters;
    filters << "*.mp3" << "*.flac" << "*.wav";
    QDir dir(defaultMusicPath);
    dir.setNameFilters(filters);
    musicFiles = dir.entryList(QDir::Files);
    for (int i = 0; i < musicFiles.size(); ++i) {
        musicFiles[i] = dir.absoluteFilePath(musicFiles.at(i));
    }
}

void music_player::playMusic(const QString &filePath)
{
    // 开始播放歌曲后设置可以人为拖动进度条
    ui->progress->setEnabled(true);

    if (mplayerProcess->state() == QProcess::Running) {
        mplayerProcess->terminate(); // 终止当前正在运行的 mplayer 进程
        if (!mplayerProcess->waitForFinished(3000)) { // 等待进程终止，超时3秒
            mplayerProcess->kill();
        }
    }

    mplayerProcess->start("mplayer", QStringList() << "-slave" << "-quiet" << "-novideo" << filePath);

    if (!mplayerProcess->waitForStarted()) {
        printf("mplayer error\r\n");
        return;
    }

    play_flag = true;
    pointer_timer->start(10);
    cd_timer->start(5);
    ui->start_pause->setIcon(QIcon(":/images/start.svg"));

    // 获取歌曲名称并设置到 QLabel
    QString songName = QFileInfo(filePath).completeBaseName();
    ui->label_song_name->setText(songName);
}


void music_player::on_file_list_clicked()
{
    printf("on_file_list_clicked\r\n");

#ifdef __arm__
    currentFilePath = QFileDialog::getOpenFileName(this, "Open File", defaultMusicPath, "Music(*.mp3 *.flac *.wav)");
#else
    currentFilePath = QFileDialog::getOpenFileName(this, "Open File", "./music", "Music(*.mp3 *.flac *.wav)");
#endif

    if (!currentFilePath.isEmpty()) {
        qDebug() << currentFilePath << endl;
        currentIndex = musicFiles.indexOf(currentFilePath);
        playMusic(currentFilePath);
    }
}



void music_player::on_start_pause_clicked()
{
    printf("on_start_pause_clicked\r\n");

#ifdef __arm__
    if (mplayerProcess->state() != QProcess::Running) {
        if (musicFiles.isEmpty()) {
            QMessageBox::warning(this, "No Music", "No music files found in the default directory.");
            return;
        }

        int randomIndex = QRandomGenerator::global()->bounded(musicFiles.size());
        currentFilePath = musicFiles.at(randomIndex);
        currentIndex = randomIndex;

        playMusic(currentFilePath);
    } else {
        mplayerProcess->write("pause\n");
        if(play_flag == true){
            cd_timer->stop();
            ui->start_pause->setIcon(QIcon(":/images/pause.svg"));
            ui->start_pause->setIconSize(QSize(48, 48));
            play_flag = false;
        }
        else if(play_flag == false){
            cd_timer->start(10);
            ui->start_pause->setIcon(QIcon(":/images/start.svg"));
            ui->start_pause->setIconSize(QSize(48, 48));
            play_flag = true;
        }
        pointer_timer->start(5);
    }
#else
    if(play_flag == true){
        cd_timer->stop();
        ui->start_pause->setIcon(QIcon(":/images/pause.svg"));
        ui->start_pause->setIconSize(QSize(48, 48));
        play_flag = false;
    }
    else if(play_flag == false){
        cd_timer->start(10);
        ui->start_pause->setIcon(QIcon(":/images/start.svg"));
        ui->start_pause->setIconSize(QSize(48, 48));
        play_flag = true;
    }
    pointer_timer->start(5);
#endif
}

void music_player::on_volume_clicked()
{
    // 这里你可以添加音量调节的代码
    printf("on_volume_clicked\r\n");

    if (ui->volume_Slider->isVisible()) {
         ui->volume_widget->setVisible(false);
     } else {
         ui->volume_widget->setVisible(true);
     }
}

void music_player::on_next_clicked()
{
    printf("on_next_clicked\r\n");

    if (musicFiles.isEmpty()) {
        QMessageBox::warning(this, "No Music", "No music files found in the default directory.");
        return;
    }

    if (currentMode == Sequential) {
        currentIndex = (currentIndex + 1) % musicFiles.size();
    } else if (currentMode == SingleRepeat) {
        // currentIndex remains the same
    } else if (currentMode == Shuffle) {
        currentIndex = QRandomGenerator::global()->bounded(musicFiles.size());
    }

    currentFilePath = musicFiles.at(currentIndex);
    playMusic(currentFilePath);
}

void music_player::on_previous_clicked()
{
    printf("on_previous_clicked\r\n");

    if (musicFiles.isEmpty()) {
        QMessageBox::warning(this, "No Music", "No music files found in the default directory.");
        return;
    }

    if (currentMode == Sequential) {
        currentIndex = (currentIndex - 1 + musicFiles.size()) % musicFiles.size();
    } else if (currentMode == SingleRepeat) {
        // currentIndex remains the same
    } else if (currentMode == Shuffle) {
        currentIndex = QRandomGenerator::global()->bounded(musicFiles.size());
    }

    currentFilePath = musicFiles.at(currentIndex);
    playMusic(currentFilePath);

}


void music_player::on_music_player_back_clicked()
{
    printf("on_music_player_back_clicked\r\n");
    cd_timer->stop();
    pointer_timer->stop();
    progressTimer->stop();
    // 退出时恢复默认音量
    ui->volume_Slider->setValue(50);

    if (mplayerProcess->state() == QProcess::Running) {
        mplayerProcess->write("quit\n");
        mplayerProcess->waitForBytesWritten(1000);
        mplayerProcess->waitForFinished(3000);  // 等待mplayer进程结束
    }

    this->close();

    MainWindow *m = new MainWindow();
    m->show();
}


void music_player::on_mode_clicked()
{
    printf("on_mode_clicked\r\n");
    if (currentMode == Sequential) {
       currentMode = SingleRepeat;
       ui->mode->setIcon(QIcon(":/images/single_repeat.svg"));
       ui->mode->setIconSize(QSize(32, 32));
    } else if (currentMode == SingleRepeat) {
       currentMode = Shuffle;
       ui->mode->setIcon(QIcon(":/images/shuffle.svg"));
       ui->mode->setIconSize(QSize(32, 32));
    } else {
       currentMode = Sequential;
       ui->mode->setIcon(QIcon(":/images/sequential.svg"));
       ui->mode->setIconSize(QSize(32, 32));
    }
}

void music_player::on_volume_Slider_valueChanged(int value)
{
    printf("on_volume_Slider_valueChanged, %d\r\n", value);
    ui->volume_label->setText(QString("%1%").arg(value));
#ifdef __arm__
    // 将滑块的值转换为音量百分比
    int volume = value;

    // 发送音量调节命令给 mplayer
    QString command = QString("volume %1 1\n").arg(volume);
    mplayerProcess->write(command.toUtf8());

    QByteArray byteArray = QString("amixer sset Speaker %1,%1").arg((float)volume*127.0/100.0).toUtf8();
    const char* cmd = byteArray.data();

    system(cmd);

#endif
    volume_timer->start(2000);
}

void music_player::on_progress_sliderPressed()
{
    printf("on_progress_sliderPressed\r\n");
    sliderBeingDragged = true;
}

void music_player::on_progress_sliderReleased()
{
    printf("on_progress_sliderReleased\r\n");
    sliderBeingDragged = false;
    double newPosition = (double)ui->progress->value() / 100 * duration;
    QString command = QString("seek %1 2\n").arg(newPosition);
    mplayerProcess->write(command.toUtf8());
    if(!play_flag) mplayerProcess->write("pause\n");
}

void music_player::on_progress_valueChanged(int value)
{
    printf("on_progress_valueChanged, %d\r\n", value);
    if (sliderBeingDragged) {
       int newPosition = (double)value / 100 * duration;
       QString label_time = QString("%1%2:%3%4 / %5%6:%7%8")
                            .arg(newPosition / 60 / 10).arg(newPosition / 60 % 10)
                            .arg(newPosition % 60 / 10).arg(newPosition % 60 % 10)
                            .arg(duration / 60 / 10).arg(duration / 60 % 10)
                            .arg(duration % 60 / 10).arg(duration % 60 % 10);

       ui->label_progress->setText(label_time);
   }
}
