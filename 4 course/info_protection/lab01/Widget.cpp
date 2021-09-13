#include "Widget.h"
#include "./ui_Widget.h"
#include <cmath>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , zabor(new QGraphicsLineItem())
    , zdanie(new QGraphicsRectItem())
    , window_1(new QGraphicsLineItem())
    , window_2(new QGraphicsLineItem())
    , info(new QGraphicsRectItem())
    , hole(new QGraphicsLineItem())

{
    ui->setupUi(this);
    scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);

    scene->addItem(zabor);
    scene->addItem(zdanie);
    scene->addItem(window_1);
    scene->addItem(window_2);
    scene->addItem(info);
    scene->addItem(hole);

    // обводка
    QPen pen(Qt::SolidLine);
    pen.setWidth(penWidth*multiplyFactor);
    pen.setColor(QColor(Qt::black));
    pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);

    // координатные прямые x и y
    const int offset = 10;
    scene->addLine(-offset, maxY*multiplyFactor + offset, maxX*multiplyFactor + offset, maxY*multiplyFactor + offset, Qt::DashLine);
    scene->addLine(-offset, -offset, -offset, maxY*multiplyFactor + offset, Qt::DashLine);

    // начальное состояние забора
    ui->spinBox_zabor_x->setValue(50);
    ui->spinBox_zabor_y->setValue(50);
    ui->spinBox_zabor_x->setMinimum(windowSize*2 + windowBetweenSize + penWidth*2);
    ui->spinBox_zabor_y->setMinimum(penWidth*2 + infoSize);
    zabor->setPen(pen);

    // начальное состояние здания
    QPen zdaniePen(pen);
    zdaniePen.setColor(QColor(Qt::lightGray));
    ui->spinBox_zdanie_t1_x->setValue(10);
    ui->spinBox_zdanie_t1_y->setValue(10);
    ui->spinBox_zdanie_t2_x->setValue(40);
    ui->spinBox_zdanie_t2_y->setValue(40);
    zdanie->setPen(zdaniePen);

    // окна
    // обводка окон
    QPen windowPen(pen);
    windowPen.setColor(QColor(Qt::blue));

    window_1->setPen(windowPen);
    window_2->setPen(windowPen);
    ui->spinBox_window_1_x->setValue(14);
    ui->spinBox_window_2_x->setValue(27);

    // источник информации
    QPen infoPen(pen);
    infoPen.setColor(QColor(Qt::darkGreen));
    info->setPen(infoPen);

    info->setBrush(QBrush(QColor(Qt::green)));
    info->setRect(0, 0,
                  (infoSize - penWidth) * multiplyFactor,
                  (infoSize - penWidth) * multiplyFactor
                  );
    ui->spinBox_info_x->setValue(15);
    ui->spinBox_info_y->setValue(25);

    // брешь в заборе
    hole->setPen(zdaniePen);
    ui->doubleSpinBox_enter->setValue(24.5);

    auto *timer = new QTimer();
    timer->setSingleShot(true);
    // Подключаем СЛОТ для отрисовки к таймеру
    connect(timer, &QTimer::timeout, this, &Widget::scaleGraphicsView);
    timer->start(100);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_doubleSpinBox_enter_valueChanged(double arg1)
{
    const double step = 0.5;
    const double value = ceil(arg1/step) * step;
    ui->doubleSpinBox_enter->setValue(value);

    const double y = ui->spinBox_zabor_y->value();

    hole->setLine(value * multiplyFactor, (maxY - y) * multiplyFactor, (value + step) * multiplyFactor, (maxY - y) * multiplyFactor);
}

void Widget::resizeEvent(QResizeEvent *event)
{
    this->scaleGraphicsView();
}

void Widget::scaleGraphicsView()
{
    QRectF rect = scene->sceneRect();
    rect.setWidth(rect.width() * 1.3);
    rect.setHeight(rect.height() * 1.3);
    ui->graphicsView->fitInView(rect, Qt::KeepAspectRatio);
    ui->graphicsView->centerOn(0,0);
}

void Widget::calculateOdds()
{
    // расстояние с улицы до окна 1
    const double x_hole = ui->doubleSpinBox_enter->value();
    const double y_hole = ui->spinBox_zabor_y->value();
    const double x_window_1 = ui->spinBox_window_1_x->value();
    const double y_window = ui->spinBox_zdanie_t2_y->value();

    const double x_diff_hole_window_1 = (x_hole - x_window_1) * (x_hole - x_window_1);
    const double y_diff_hole_window = (y_hole - y_window) * (y_hole - y_window);

    const double l_outside_to_window_1 = sqrt(x_diff_hole_window_1 + y_diff_hole_window);

    // расстояние от окна 1 до инфы
    const double x_info = ui->spinBox_info_x->value();
    const double y_info = ui->spinBox_info_y->value();

    const double x_diff_window_1_info = (x_window_1 - x_info) * (x_window_1 - x_info);
    const double y_diff_window_info = (y_window - y_info) * (y_window - y_info);

    const double l_window_1_to_info = sqrt(x_diff_window_1_info + y_diff_window_info);

    const double P1 = (k1 / l_outside_to_window_1) * ui->doubleSpinBox_odds->value() * (k2/ l_window_1_to_info);
    ui->lineEdit_res->setText(QString::number(P1,'f'));

    // расстояние с улицы до окна 2
    const double x_window_2 = ui->spinBox_window_2_x->value();
    const double x_diff_hole_window_2 = (x_hole - x_window_2) * (x_hole - x_window_2);
    const double l_outside_to_window_2 = sqrt(x_diff_hole_window_2 + y_diff_hole_window);

    // расстояние от окна 2 до инфы
    const double x_diff_window_2_info = (x_window_2 - x_info) * (x_window_2 - x_info);
    const double l_window_2_to_info = sqrt(x_diff_window_2_info + y_diff_window_info);

    const double P2 = (k1 / l_outside_to_window_2) * ui->doubleSpinBox_odds_2->value() * (k2/ l_window_2_to_info);
    ui->lineEdit_res_2->setText(QString::number(P2,'f'));

}

void Widget::on_spinBox_zabor_x_valueChanged(int arg1)
{
    auto newLine = zabor->line();
    newLine.setP2(QPointF(arg1 * multiplyFactor, newLine.p2().y()));
    zabor->setLine(newLine);

    ui->spinBox_zdanie_t2_x->setMaximum(arg1);

    ui->doubleSpinBox_enter->setMaximum(arg1);
}

void Widget::on_spinBox_zabor_y_valueChanged(int arg1)
{
    auto newLine = zabor->line();
    newLine.setP1(QPointF(newLine.p1().x(), (maxY-arg1) * multiplyFactor));
    newLine.setP2(QPointF(newLine.p2().x(), (maxY-arg1) * multiplyFactor));
    zabor->setLine(newLine);

    ui->spinBox_zdanie_t2_y->setMaximum(arg1-penWidth);

    on_doubleSpinBox_enter_valueChanged(ui->doubleSpinBox_enter->value());
}

void Widget::on_spinBox_zdanie_t1_x_valueChanged(int arg1)
{
    auto newRect = zdanie->rect();
    newRect.setTopLeft(QPointF(arg1*multiplyFactor, newRect.topLeft().y()));
    zdanie->setRect(newRect);
    ui->spinBox_zdanie_t2_x->setMinimum(arg1 + (1 + windowSize * 2 + windowBetweenSize));
    ui->spinBox_window_1_x->setMinimum(arg1 + (windowSize-1)/2 + 1);

    ui->spinBox_info_x->setMinimum(arg1+2);
}

void Widget::on_spinBox_zdanie_t1_y_valueChanged(int arg1)
{
    auto newRect = zdanie->rect();
    newRect.setBottomLeft(QPointF(newRect.topLeft().x(), (maxY-arg1) * multiplyFactor));
    zdanie->setRect(newRect);
    ui->spinBox_zdanie_t2_y->setMinimum(arg1+penWidth+infoSize);

    ui->spinBox_info_y->setMinimum(arg1+2);
}

void Widget::on_spinBox_zdanie_t2_x_valueChanged(int arg1)
{
    auto newRect = zdanie->rect();
    newRect.setTopRight(QPointF(arg1*multiplyFactor, newRect.topRight().y()));
    zdanie->setRect(newRect);
    ui->spinBox_zdanie_t1_x->setMaximum(arg1-(1 + windowSize*2 + windowBetweenSize));

    const int window_2_max = (arg1 - ((windowSize-1) / 2) - penWidth);
    if (window_2_max > 0)
    {
        ui->spinBox_window_1_x->setMaximum(window_2_max - windowSize - windowBetweenSize);
        ui->spinBox_window_2_x->setMaximum(window_2_max);
    }

    ui->spinBox_info_x->setMaximum(arg1-2);
}

void Widget::on_spinBox_zdanie_t2_y_valueChanged(int arg1)
{
    auto newRect = zdanie->rect();
    newRect.setTopLeft(QPointF(newRect.topLeft().x(), (maxY-arg1) * multiplyFactor));
    zdanie->setRect(newRect);
    ui->spinBox_zdanie_t1_y->setMaximum(arg1-penWidth-infoSize);

    // обновляем местоположение окон
    on_spinBox_window_1_x_valueChanged(ui->spinBox_window_1_x->value());
    on_spinBox_window_2_x_valueChanged(ui->spinBox_window_2_x->value());

    ui->spinBox_info_y->setMaximum(arg1-2);
}

void Widget::on_spinBox_window_1_x_valueChanged(int arg1)
{
    const int y = maxY - ui->spinBox_zdanie_t2_y->value();
    const int halfWindowSize = (windowSize-1)/2;

    window_1->setLine((arg1 - halfWindowSize) * multiplyFactor,
                      y * multiplyFactor,
                      (windowSize + (arg1-halfWindowSize-penWidth)) * multiplyFactor,
                      y * multiplyFactor);

    ui->spinBox_window_2_x->setMinimum(arg1+windowSize+windowBetweenSize);
}

void Widget::on_spinBox_window_2_x_valueChanged(int arg1)
{
    const int y = maxY - ui->spinBox_zdanie_t2_y->value();

    const int halfWindowSize = (windowSize-1)/2;
    window_2->setLine((arg1-halfWindowSize) * multiplyFactor,
                      y * multiplyFactor,
                      (windowSize + (arg1-halfWindowSize-penWidth)) * multiplyFactor,
                      y * multiplyFactor);
}

void Widget::on_spinBox_info_x_valueChanged(int arg1)
{
    info->setPos((arg1 - (infoSize-1)/2) * multiplyFactor, info->pos().y());
}

void Widget::on_spinBox_info_y_valueChanged(int arg1)
{
    info->setPos(info->pos().x(), (maxY - arg1 - (infoSize-1)/2) * multiplyFactor);
}


void Widget::on_pushButton_calculate_clicked()
{
    this->calculateOdds();
}

