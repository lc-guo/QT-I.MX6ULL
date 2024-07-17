#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <QMainWindow>
#include <QProcess>


namespace Ui { class music_player; }

class music_player : public QMainWindow
{
    Q_OBJECT
public:
    explicit music_player(QWidget *parent = nullptr);
    ~music_player();

signals:

private slots:
    void on_music_player_back_clicked();
    void on_file_list_clicked();
    void on_volume_clicked();
    void on_start_pause_clicked();
    void handleMPlayerOutput();
    void handleMPlayerError();
    void on_next_clicked();
    void on_previous_clicked();
    void on_mode_clicked();
    void on_volume_Slider_valueChanged(int value);
    void on_progress_sliderPressed();
    void on_progress_sliderReleased();
    void on_progress_valueChanged(int value);

private:
    Ui::music_player *ui;
    bool play_flag;

    QProcess *mplayerProcess;
    QString currentFilePath;
    QString defaultMusicPath;
    QStringList musicFiles;
    int currentIndex;
    QTimer *progressTimer;
    QTimer *cd_timer;
    QTimer *pointer_timer;
    QTimer *volume_timer;
    int duration;
    int currentPosition;
    int currentProgressValue;
    QString currentProgressLabel;
    int cd_angle;
    int pointer_angle;
    QPixmap cd_image;
    QPixmap pointer_image;
    bool sliderBeingDragged;


    enum PlayMode {
        Sequential,
        SingleRepeat,
        Shuffle
    };

    PlayMode currentMode;

    void loadMusicFiles();
    void playMusic(const QString &filePath);
    void updateProgress();
    void updateCD();
    void updatePointer();
};

#endif // MUSIC_PLAYER_H
