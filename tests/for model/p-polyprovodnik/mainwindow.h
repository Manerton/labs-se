#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "picturebox.h" // фрейм, в котором все рисуем
#include <QElapsedTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void hideInputs();

signals:
    void signalPauseAnimation();
private slots:
    void on_pushButton_2_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_pushButton_clicked();
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    PictureBox *graphic;
};


#endif // MAINWINDOW_H
