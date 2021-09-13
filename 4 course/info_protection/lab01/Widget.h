#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_doubleSpinBox_enter_valueChanged(double arg1);

    void on_spinBox_zabor_x_valueChanged(int arg1);

    void on_spinBox_zabor_y_valueChanged(int arg1);

    void on_spinBox_zdanie_t1_x_valueChanged(int arg1);

    void on_spinBox_zdanie_t1_y_valueChanged(int arg1);

    void on_spinBox_zdanie_t2_y_valueChanged(int arg1);

    void on_spinBox_zdanie_t2_x_valueChanged(int arg1);

    void on_spinBox_window_1_x_valueChanged(int arg1);

    void on_spinBox_window_2_x_valueChanged(int arg1);

    void on_spinBox_info_x_valueChanged(int arg1);

    void on_spinBox_info_y_valueChanged(int arg1);

    void on_pushButton_calculate_clicked();

private:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
// поля
    Ui::Widget *ui;
    QGraphicsScene *scene;
    static constexpr int multiplyFactor = 6;
    static constexpr int maxY = 50;
    static constexpr int maxX = 50;
    static constexpr double k1 = 2.0;
    static constexpr double k2 = 0.5;
    QGraphicsLineItem *zabor;
    QGraphicsRectItem *zdanie;
    static constexpr int penWidth = 1;
    // должен быть нечетным, чтобы был серединный пиксель
    static constexpr int windowSize = 3;
    static constexpr int windowBetweenSize = 1;
    // должен быть нечетным, чтобы был серединный пиксель
    static constexpr int infoSize = 3;
    QGraphicsLineItem *window_1;
    QGraphicsLineItem *window_2;
    QGraphicsRectItem *info;
    QGraphicsLineItem *hole;
// методы
    void scaleGraphicsView();
    void calculateOdds();
};
#endif // WIDGET_H
