#include "Widget.h"
#include "./ui_Widget.h"

#include <cmath>
#include "thirdparty/qcustomplot.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // готовим графики
    this->prepareChart(ui->widget_chart);
    this->prepareChart(ui->widget_chart_crypt);
    this->prepareChart(ui->widget_chart_decode);

    ui->widget_chart->plotLayout()->insertRow(0);
    ui->widget_chart->plotLayout()->addElement(0, 0, new QCPTextElement(ui->widget_chart, "Оригинальный сигнал", 12));

    ui->widget_chart_crypt->plotLayout()->insertRow(0);
    ui->widget_chart_crypt->plotLayout()->addElement(0, 0, new QCPTextElement(ui->widget_chart_crypt, "Зашифрованный сигнал", 12));

    ui->widget_chart_decode->plotLayout()->insertRow(0);
    ui->widget_chart_decode->plotLayout()->addElement(0, 0, new QCPTextElement(ui->widget_chart_decode, "Расшифрованный сигнал", 12));

    // запрещаем вводить символы кроме цифр в поле для ключа
    auto valid = new QRegularExpressionValidator(QRegularExpression("\\d*"), this);
    ui->key_LineEdit->setValidator(valid);

    // выключаем кнопку показа
    ui->pushButton_show->setEnabled(false);
}

// вычисляем функцию из задания
QVector<Widget::interval> Widget::calc_func() const
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
void Widget::render_func(QCustomPlot *customplot, const QVector<interval>& data) const
{
    //areas*2 так как включаются и точки начала и точки конца для areas интервалов, т.е areas*2 точек

    // для рендера x
    QVector<double> keys;
    keys.reserve(areas*2);

    // для рендера y
    QVector<double> values;
    values.reserve(areas*2);

    for (int t = 0; t < areas; ++t)
    {
        keys.push_back(t);
        values.push_back(data[t].y1());

        keys.push_back(t+1);
        values.push_back(data[t].y2());
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

// получаем порядок интервалов из ключа
QVector<int> Widget::get_key_intervals() const
{
    QVector<int> key_intervals;
    key_intervals.reserve(areas);

    QString key = ui->key_LineEdit->text();

    for (QChar c : key)
    {
        key_intervals.push_back(c.digitValue());
    }

    return key_intervals;
}

QVector<Widget::interval> Widget::crypt_data(const QVector<interval> &data, const QVector<int>& key_intervals) const
{
    QVector<interval> crypted_data(areas);

    // зашифровываем по ключу
    for (int i = 0; i < areas; ++i)
    {
        crypted_data[i] = data[key_intervals[i]];
    }

    return crypted_data;
}

QVector<Widget::interval> Widget::decode_data(const QVector<interval> &crypted_data, const QVector<int>& key_intervals) const
{
    QVector<interval> decoded_data(areas);

    // расшифровываем по ключу
    for (int i = 0; i < areas; ++i)
    {
        decoded_data[key_intervals[i]] = crypted_data[i];
    }

    return decoded_data;
}

void Widget::on_pushButton_show_clicked()
{
    // получаем интервалы (оригинальный сигнал)
    auto orig_data = calc_func();
    this->render_func(ui->widget_chart, orig_data);

    // получить порядок интервалов из ключа
    auto key_intervals = get_key_intervals();

    // зашифровываем сигнал
    auto crypted_data = crypt_data(orig_data, key_intervals);
    this->render_func(ui->widget_chart_crypt, crypted_data);

    // расшифровываем сигнал
    auto decoded_data = decode_data(crypted_data, key_intervals);
    this->render_func(ui->widget_chart_decode, decoded_data);
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

