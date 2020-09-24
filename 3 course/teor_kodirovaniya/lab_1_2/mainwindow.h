#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "probability_scheme.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    void setupGraph();
    std::vector<probability_scheme> vector_schemes; // вероятностные схемы для p от 0 до 1
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void show_probality_scheme();

public slots:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_doubleSpinBox_p_valueChanged(double arg1);

    void on_pushButton_grafik_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
