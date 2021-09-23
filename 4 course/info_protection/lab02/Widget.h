#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QCustomPlot;

class Widget : public QWidget
{
    Q_OBJECT
public:
    // интервал на графике (две точки)
    using interval = QLineF;
private:
    // количество интервалов
    const uint16_t areas = 10;
    // функция из задания
    QVector<interval> calc_func();
    // отрисовать функцию на графике customplot, с порядком интервалов key
    void render_func(QCustomPlot *customplot, const QVector<int> &key);
    // подготовить графики (диапазон оси x и так далее)
    void prepareChart(QCustomPlot* customplot) const;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_show_clicked();
    void on_key_LineEdit_textEdited(const QString &arg1);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
