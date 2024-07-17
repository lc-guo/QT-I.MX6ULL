#ifndef RECORDING_H
#define RECORDING_H

#include <QMainWindow>

namespace Ui { class recording; }

class recording : public QMainWindow
{
    Q_OBJECT
public:
    explicit recording(QWidget *parent = nullptr);
    ~recording();

signals:

private slots:
    void on_recording_back_clicked();

private:
    Ui::recording *ui;

};

#endif // RECORDING_H
