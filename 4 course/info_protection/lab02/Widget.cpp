#include "Widget.h"
#include "./ui_Widget.h"

#include <cmath>
#include "thirdparty/qcustomplot.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // готовим графики
    this->prepareChart(ui->widget_chart);
    this->prepareChart(ui->widget_chart_crypt);

    // запрещаем вводить символы кроме цифр в поле для ключа
    QRegExpValidator* valid = new QRegExpValidator(QRegExp("\\d*"), this);
    ui->key_LineEdit->setValidator(valid);

    // выключаем кнопку показа
    ui->pushButton_show->setEnabled(false);
}

// вычисляем функцию из задания
QVector<Widget::interval> Widget::calc_func()
{
    // константы из полей
    const double A = ui->a_DoubleSpinBox->value();
    const double B = ui->b_DoubleSpinBox->value();
    const double C = ui->c_DoubleSpinBox->value();

    const double alpha = ui->alpha_DoubleSpinBox->value();
    const double beta = ui->beta_DoubleSpinBox->value();
    const double gamma = ui->gamma_DoubleSpinBox->value();

    // сама функция
    auto func = [&](double t)
    {
        QPointF res = { t, A * sin(alpha * t) + B * cos(beta * t) + t * C * cos( cos(gamma * t) )};
        return res;
    };

    // интервалы
    QVector<interval> charts(areas);
    for (int t = 0; t < areas; ++t)
    {
        QPointF start = func(t);
        QPointF end = func(t+1);
        charts[t] = interval(start, end);
    }

    return charts;
}

// отрисовать функцию на графике customplot, с порядком интервалов key
void Widget::render_func(QCustomPlot *customplot, const QVector<int> &key)
{
    // получаем интервалы
    auto data = calc_func();

    //areas*2 так как включаются и точки начала и точки конца для areas интервалов, т.е areas*2 точек

    // для рендера x
    QVector<double> keys;
    keys.reserve(areas*2);

    // для рендера y
    QVector<double> values;
    values.reserve(areas*2);

    // записываем игрики для t и t+1 на интервале j
    for (int t = 0; t < areas; ++t)
    {
        int j = key[t];

        keys.push_back(t);
        values.push_back(data[j].y1());

        keys.push_back(t+1);
        values.push_back(data[j].y2());
    }

    // рисуем график
    customplot->graph(0)->setData(keys, values, true);
    customplot->yAxis->rescale();
    customplot->replot();
}

void Widget::prepareChart(QCustomPlot *customplot) const
{
    customplot->addGraph();
    customplot->graph(0)->setPen(QPen(Qt::blue));
    // диапазон
    customplot->xAxis->setRange(0,areas);
    // шаг оси X
    customplot->xAxis->ticker()->setTickCount(areas);
    customplot->xAxis->setSubTicks(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_show_clicked()
{
    this->render_func(ui->widget_chart, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

    QVector<int> key_intervals;
    key_intervals.reserve(areas);

    QString key = ui->key_LineEdit->text();

    for (QChar c : key)
    {
        key_intervals.push_back(c.digitValue());
    }

    this->render_func(ui->widget_chart_crypt, key_intervals);
}

void Widget::on_key_LineEdit_textEdited(const QString &arg1)
{
    QSet<QChar> symbols(arg1.begin(), arg1.end());
    if (symbols.size() != arg1.length())
    {
        ui->key_LineEdit->backspace();
    }

    if (ui->key_LineEdit->text().length() == areas)
    {
        ui->pushButton_show->setEnabled(true);
    }
    else if (ui->pushButton_show->isEnabled())
    {
        ui->pushButton_show->setEnabled(false);
    }
}

