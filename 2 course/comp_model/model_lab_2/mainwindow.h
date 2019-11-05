#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "telo.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void action();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_left_valueChanged(int arg1);

    void on_spinBox_env_valueChanged(int arg1);

    void on_spinBox_right_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QTimer *paintTimer;
    void update_temp();
    void get_koef();
    void disable_buttons(bool var);

    telo object;
    int max_temp = 0, min_temp = 0;
    double k_temp = 1;
    int time = 3;
};

#endif // MAINWINDOW_H
