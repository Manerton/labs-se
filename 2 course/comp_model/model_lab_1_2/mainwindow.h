#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "telo.h"

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

    void on_spinBox_v_valueChanged(int arg1);

    void on_spinBox_a_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    double V0;
    double a;
    telo object;
};

#endif // MAINWINDOW_H
