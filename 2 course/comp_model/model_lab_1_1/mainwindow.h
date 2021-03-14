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
protected:
    virtual void paintEvent(QPaintEvent *);
private slots:
    void on_pushButton_clicked();
    void get_new_ycoord();

    void on_SpinBox_v_valueChanged(double arg1);

    void on_SpinBox_h_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    double V0;
    double h;
    double ycoord = 0;
    int i = 0;
    telo object;
    QTimer paintTimer; // таймер
};

#endif // MAINWINDOW_H
