#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "picturebox.h" // фрейм, в котором все рисуем

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    PictureBox *pb_frame; // указатель на фрейм
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pushButton_draw_clicked(); // нарисовать

    void on_pushButton_disable_clicked();

    void on_comboBox_lens_currentIndexChanged(const QString &arg1);

    void on_comboBox_pic_currentIndexChanged(const QString &arg1);

private:
    double getAngle (const double &gradus) const; // получить радианы из градусов
};


#endif // MAINWINDOW_H
