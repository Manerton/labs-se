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
    QVector<interval> calc_func() const;
    // отрисовать функцию на графике customplot
    void render_func(QCustomPlot *customplot, const QVector<interval> &data) const;
    // подготовить графики (диапазон оси x и так далее)
    void prepareChart(QCustomPlot* customplot) const;
    // зашифровать по ключу
    QVector<interval> crypt_data(const QVector<interval> &data, const QVector<int>& key_intervals) const;
    // расшифровать по ключу
    QVector<interval> decode_data(const QVector<interval> &crypted_data, const QVector<int>& key_intervals) const;
    // получить порядок интервалов из ключа
    QVector<int> get_key_intervals() const;
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
