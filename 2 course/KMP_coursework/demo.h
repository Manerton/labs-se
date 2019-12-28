#ifndef DEMO_H
#define DEMO_H

#include <QMainWindow>
#include <QElapsedTimer>
#include <QCoreApplication>
#include "picturebox.h"
namespace Ui {
class Demo;
}

class Demo : public QMainWindow
{
    Q_OBJECT
    std::vector<QString> code;                      // -- коды, отображаемые при демонстрации -- //
    PictureBox *pb_frame = nullptr;
public:
    explicit Demo(QWidget *parent = nullptr);
    ~Demo();
    inline static void wait(int ms)
    {
        QElapsedTimer timer;
        timer.start();
        while (timer.elapsed() < ms){}
    }

signals:
    void return_to_menu();

private slots:
    void on_pushButton_start_clicked();
    void on_pushButton_result_clicked();
    void on_pushButton_step2_clicked();
    void on_pushButton_finish_clicked();
    void on_lineEdit_str_textChanged(const QString &arg1);

private:
    Ui::Demo *ui;
    void keyPressEvent(QKeyEvent *event);           // -- обработка клавиш -- //
    void update_code();
};
#endif // DEMO_H
