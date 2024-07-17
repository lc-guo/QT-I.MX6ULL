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

private:
    Ui::video *ui;
    bool play_flag;
    QProcess *mplayerProcess;
    QString currentFilePath;
    QString defaultVideoPath;
    QStringList videoFiles;
    int currentIndex;

    void mousePressEvent(QMouseEvent *event);
    void loadMusicFiles();
    void playVideo(const QString &filePath);
};

#endif // VIDEO_H
