#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "animal.h"
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
    void on_SpinBox_v_valueChanged(double arg1);
    void on_SpinBox_l_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    double v;
    double l;
    animal crocodile;
};

#endif // MAINWINDOW_H
