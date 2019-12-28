#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_teoria_clicked();

    void return_to_menu();

    void on_pushButton_test_clicked();

    void on_pushButton_demo_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
