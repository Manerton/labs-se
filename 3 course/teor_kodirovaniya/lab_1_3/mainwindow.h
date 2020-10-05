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
    std::vector<probability_scheme> vector_schemes; // вероятностные схемы для k от 0 до n
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void show_probality_scheme();

public slots:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_spinBox_k_valueChanged(int);       // изменение числа в поле для ввода k
    void on_pushButton_grafik_clicked();            // после нажатия на кнопку заполняем vector_schemes, и вызываем show_probality_scheme и setupGraph

    void on_spinBox_m_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
