#ifndef VIDEO_H
#define VIDEO_H

#include <QMainWindow>
#include <QProcess>

namespace Ui { class video; }

class video : public QMainWindow
{
    Q_OBJECT
public:
    explicit video(QWidget *parent = nullptr);
    ~video();

signals:


private slots:
    void on_video_back_clicked();
    void on_start_pause_clicked();
    void handleMPlayerOutput();
    void handleMPlayerError();
    void on_file_list_clicked();
    void on_next_clicked();
    void on_previous_clicked();
    void on_volume_Slider_valueChanged(int value);

private:
    Ui::video *ui;
    bool play_flag;
    QProcess *mplayerProcess;
    QString currentFilePath;
    QString defaultVideoPath;
    QStringList videoFiles;
    int currentIndex;
    QTimer *volume_timer;

    void mousePressEvent(QMouseEvent *event);
    void loadvideoFiles();
    void playVideo(const QString &filePath);
};

#endif // VIDEO_H
