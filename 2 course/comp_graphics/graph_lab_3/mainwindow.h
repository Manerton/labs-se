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
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void startFractal(int depth); // начать рисование фрактала с заданной глубинной
    void getCoords(coord a, int depth); // рекурсивная функция получения координат фигуры-H
    void getColors(int depth); // рандомный цвет для каждого уровня глубины
private slots:
    void on_spinBox_valueChanged(int arg1);
private:
    Ui::MainWindow *ui;
    PictureBox *pb_frame; // указатель на фрейм
    int sizeMin = 260; // начальная длина
    QVector<coord> coords; // вектор, используемый для запоминания точек в которых надо рисовать Н
    QVector<QColor> colors; // цвета
};


#endif // MAINWINDOW_H
