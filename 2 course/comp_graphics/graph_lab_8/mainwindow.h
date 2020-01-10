#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include <QCoreApplication>
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
    inline static void wait(int ms)
    {
        QElapsedTimer timer;
        timer.start();
        do {
            QCoreApplication::processEvents(QEventLoop::AllEvents, ms);
            QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
        } while (timer.elapsed() < ms);
    }

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pushButton_draw_clicked(); // нарисовать
};


#endif // MAINWINDOW_H
