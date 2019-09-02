#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addPoint(int x, int y);
    void plot();

private:
    Ui::MainWindow *ui;

    QVector<double> qv_x, qv_y;
};

#endif // MAINWINDOW_H
