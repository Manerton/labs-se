#include <cmath>
#include <QDebug>
#include "picturebox.h"
#include "demo.h"

using namespace std;

PictureBox::PictureBox(QWidget *parent) : QFrame(parent)
{
    resize(parent->width()-2, parent->height()-2);
    m_Pixmap = QPixmap(size()); // инициализирую полотно, на котором буду рисовать
    m_Pixmap.fill(Qt::white); // заливаю белым цветом
}

void PictureBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(1,1,m_Pixmap);
}

PictureBox::drawSettings PictureBox::prepare_drawSettings(int length)    // -- подготовка настроек для демонстрации работы префиксной функции -- //
{
    // -- настройки шрифта -- //
    QFont font;                         // -- для текста -- //
    font.setFamily("Times New Roman");
    font.setPixelSize(32);
    QFont font_for_num = font;          // -- для чисел -- //
    font_for_num.setPixelSize(20);
    font_for_num.setBold(true);
    // ---------------------- //
    int cell_size = 40;                 // -- размер ячейки -- //
    int h = height()/2;
    int w = width()/2;
    int x = 0;
    if (length % 2 == 0)                   // -- если количество символов четное -- //
    {
         x = w - (length/2) * cell_size;
    }
    else                                // -- если нечетное, то делаем отступ в пол-квадрата для символа -- //
    {
         x = w - (length/2) * cell_size - cell_size/2;
    }
    const int y = h - 20;
    return drawSettings{font,font_for_num,cell_size,QPoint{x,y}};  // -- возвращаю настройки -- //
}

void PictureBox::drawPrefFunc(QPainter &painter)   // -- демонстрация работы префикс-функции -- //
{
    m_Pixmap.fill(Qt::white);                           // -- очищаю пиксмап -- //
    const QFont font = settings.font;                   // -- получаю шрифты и так далее -- //
    const QFont font_for_num = settings.font_for_num;
    const int cell_size = settings.cell_size;
    size_t len = size_t(substr.length());
    int x = settings.p.rx();
    int y = settings.p.ry();
    for (size_t i = 0; i < len; ++i)                       // -- рисую для каждой буквы -- //
    {
        QColor char_color = Qt::black;
        QColor char_background_color = Qt::white;
        if (i == size_t(pref_steps[i_step].i) || i == size_t(pref_steps[i_step].j))
        {
            char_color = Qt::magenta;
            char_background_color = Qt::lightGray;
        }
        // -- рисуем квадрат-ячейку -- //
        painter.setPen(Qt::black);
        painter.setBrush(char_background_color);
        painter.drawRect(x,y,cell_size,cell_size);
        // -- рисуем номер над ячейкой -- //
        painter.setFont(font_for_num);
        painter.setPen(Qt::blue);
        painter.drawText(x,y-32,cell_size,cell_size,Qt::AlignCenter,QString::number(i));
        // -- рисуем символ-букву -- //
        painter.setFont(font);
        painter.setPen(char_color);
        painter.drawText(x,y,cell_size,cell_size,Qt::AlignCenter,{substr[int(i)]});
        // -- переходим к следующей -- //
        x += cell_size;
    }
    // -- рисуем значение Pi-функции под ячейкой -- //
    painter.setFont(font_for_num);
    painter.setPen(Qt::red);
    x = settings.p.rx();
    int x_Pi = x;
    painter.drawText(x_Pi,y+32,cell_size,cell_size,Qt::AlignCenter,QString::number(0));
    x_Pi += cell_size;
    for (size_t i = 1; i < size_t(pref_steps[i_step].i); ++i)
    {
        painter.drawText(x_Pi,y+32,cell_size,cell_size,Qt::AlignCenter,QString::number(Pi[i]));
        x_Pi += cell_size;
    }
    if (pref_steps[i_step].Pi != -1)
    {
        painter.drawText(x_Pi,y+32,cell_size,cell_size,Qt::AlignCenter,QString::number(pref_steps[i_step].Pi));
    }
    // -- рисуем буквы i и j -- //
    int x_j = x + cell_size * int(pref_steps[i_step].j);
    x_Pi = x + cell_size * int(pref_steps[i_step].i);
    painter.setPen(Qt::blue);
    painter.drawText(x_Pi,y-62,cell_size,cell_size,Qt::AlignCenter,"i");
    painter.drawText(x_j,y-62,cell_size,cell_size,Qt::AlignCenter,"j");
}

void PictureBox::drawPrefFunc_result(QPainter &painter)   // -- демонстрация работы префикс-функции -- //
{
    m_Pixmap.fill(Qt::white);                           // -- очищаю пиксмап -- //
    painter.setRenderHint(QPainter::Antialiasing, true);
    const QFont font = settings.font;                   // -- получаю шрифты и так далее -- //
    const QFont font_for_num = settings.font_for_num;
    const int cell_size = settings.cell_size;
    int len = substr.length();
    int x = settings.p.rx();
    int y = settings.p.ry();
    for (int i = 0; i < len; ++i)                       // -- рисую для каждой буквы -- //
    {
        QColor char_color = Qt::black;
        QColor char_background_color = Qt::white;
        if (i == int(i_step))
        {
            char_color = Qt::magenta;
            char_background_color = Qt::lightGray;
        }
        // -- рисуем квадрат-ячейку -- //
        painter.setPen(Qt::black);
        painter.setBrush(char_background_color);
        painter.drawRect(x,y,cell_size,cell_size);
        // -- рисуем номер над ячейкой -- //
        painter.setFont(font_for_num);
        painter.setPen(Qt::blue);
        painter.drawText(x,y-32,cell_size,cell_size,Qt::AlignCenter,QString::number(i));
        // -- рисуем символ-букву -- //
        painter.setFont(font);
        painter.setPen(char_color);
        painter.drawText(x,y,cell_size,cell_size,Qt::AlignCenter,{substr[i]});
        // -- переходим к следующей -- //
        x += cell_size;
    }
    // -- рисуем значение Pi-функции под ячейкой -- //
    painter.setFont(font_for_num);
    painter.setPen(Qt::red);
    x = settings.p.rx();
    int x_Pi = x;
    int N = int(Pi.size());
    for (int i = 0; i < N; ++i)
    {
        painter.drawText(x_Pi,y+32,cell_size,cell_size,Qt::AlignCenter,QString::number(Pi[size_t(i)]));
        x_Pi += cell_size;
    }
    if (Pi[i_step] > 0)
    {
        x = settings.p.rx()+5;
        y+=5;

        QPen pen;
        pen.setWidth(4);
        pen.setColor(Qt::yellow);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(x,y,int(40*Pi[i_step]),35);

        pen.setColor(Qt::green);
        painter.setPen(pen);
        x = settings.p.rx() + cell_size * int(i_step+1);
        painter.drawEllipse(x,y,int(-40*Pi[i_step]),35);

    }
}

void PictureBox::drawPiArray(QPainter &painter)   // -- вывод массива Пи -- //
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    const QFont font = settings.font;                   // -- получаю шрифты и так далее -- //
    const QFont font_for_num = settings.font_for_num;
    const int cell_size = settings.cell_size;
    int len = substr.length();
    int x = settings.p.rx();
    int y = settings.p.ry() + 50;
    for (int i = 0; i < len; ++i)                       // -- рисую для каждой буквы -- //
    {
        QColor char_color = Qt::black;
        QColor char_background_color = Qt::white;
        if (i == int(i_step))
        {
            char_color = Qt::magenta;
            char_background_color = Qt::lightGray;
        }
        // -- рисуем квадрат-ячейку -- //
        painter.setPen(Qt::black);
        painter.setBrush(char_background_color);
        painter.drawRect(x,y,cell_size,cell_size);
        // -- рисуем номер над ячейкой -- //
        painter.setFont(font_for_num);
        painter.setPen(Qt::blue);
        painter.drawText(x,y-32,cell_size,cell_size,Qt::AlignCenter,QString::number(i));
        // -- рисуем символ-букву -- //
        painter.setFont(font);
        painter.setPen(char_color);
        painter.drawText(x,y,cell_size,cell_size,Qt::AlignCenter,{substr[i]});
        // -- переходим к следующей -- //
        x += cell_size;
    }
    // -- рисуем значение Pi-функции под ячейкой -- //
    painter.setFont(font_for_num);
    painter.setPen(Qt::red);
    x = settings.p.rx();
    int x_Pi = x;
    int N = int(Pi.size());
    for (int i = 0; i < N; ++i)
    {
        painter.drawText(x_Pi,y+32,cell_size,cell_size,Qt::AlignCenter,QString::number(Pi[size_t(i)]));
        x_Pi += cell_size;
    }
    if (kmp_steps[i_step].code_i == 4)
    {
        x = settings.p.x() + cell_size * (kmp_steps[i_step-1].l - 1);
        y+=5;

        QPen pen;
        pen.setWidth(4);
        pen.setColor(Qt::yellow);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(x,y,cell_size,35);
    }
}

void PictureBox::animation_podstroka(QPainter &painter, int x, int y)
{
    int M = substr.length();
    int cell_size = settings.cell_size;
    const QFont font = settings.font;
    for (int i = 0; i < M; ++i)                       // -- рисую для каждой буквы подстроки -- //
    {
        QColor char_color = Qt::black;
        QColor char_background_color = Qt::white;
        if (i == int(kmp_steps[i_step].l))
        {
            char_color = Qt::magenta;
            char_background_color = Qt::lightGray;
        }
        // -- рисуем квадрат-ячейку -- //
        painter.setPen(Qt::black);
        painter.setBrush(char_background_color);
        painter.drawRect(x,y,cell_size,cell_size);
        // -- рисуем символ-букву -- //
        painter.setFont(font);
        painter.setPen(char_color);
        painter.drawText(x,y,cell_size,cell_size,Qt::AlignCenter,{substr[i]});
        // -- переходим к следующей -- //
        x += cell_size;
    }
}

void PictureBox::drawKMP(QPainter &painter, int anim_x)
{
    m_Pixmap.fill(Qt::white);                           // -- очищаю пиксмап -- //
    painter.setRenderHint(QPainter::Antialiasing, true);
    const QFont font = settings.font;                   // -- получаю шрифты и так далее -- //
    const QFont font_for_num = settings.font_for_num;
    const int cell_size = settings.cell_size;
    int N = str.length();
    int x = settings.p.rx();
    int y = settings.p.ry()-100;
    for (int i = 0; i < N; ++i)                       // -- рисую для каждой буквы -- //
    {
        QColor char_color = Qt::black;
        QColor char_background_color = Qt::white;
        if (i == int(kmp_steps[i_step].k))
        {
            char_color = Qt::magenta;
            char_background_color = Qt::lightGray;
        }
        // -- рисуем квадрат-ячейку -- //
        painter.setPen(Qt::black);
        painter.setBrush(char_background_color);
        painter.drawRect(x,y,cell_size,cell_size);
        // -- рисуем символ-букву -- //
        painter.setFont(font);
        painter.setPen(char_color);
        painter.drawText(x,y,cell_size,cell_size,Qt::AlignCenter,{str[i]});
        // -- переходим к следующей -- //
        x += cell_size;
    }
    x = settings.p.rx();
    y += 50;
    animation_podstroka(painter,anim_x,y);
    // -- рисуем буквы k и l -- //
    int x_k = settings.p.x() + cell_size * kmp_steps[i_step].k;
    int x_l = anim_x + cell_size * kmp_steps[i_step].l;
    painter.setPen(Qt::blue);
    painter.drawText(x_k,y-90,cell_size,cell_size,Qt::AlignCenter,"k");
    painter.drawText(x_l,y+50,cell_size,cell_size,Qt::AlignCenter,"l");
    // -- рисуем массив Pi -- //
    drawPiArray(painter);
}

void PictureBox::do_next_step_kmp(QPainter &painter)
{
    int x_l = settings.p.x() + 40 * (kmp_steps[i_step].k - kmp_steps[i_step].l);
    int prev_x_l = x_l;
    if (i_step > 0)
    {
        prev_x_l = settings.p.x() + 40 * (kmp_steps[i_step-1].k - kmp_steps[i_step-1].l);
    }
    if (i_step > 0 && prev_x_l != x_l)
    {
        for (int i = prev_x_l; i <= x_l; ++i)
        {
            Demo::wait(2);
            drawKMP(painter,i);
            repaint();
        }
    }
    else
    {
        drawKMP(painter,x_l);
    }
}

void PictureBox::do_prev_step_kmp(QPainter &painter)
{
    int x_l = settings.p.x() + settings.cell_size * (kmp_steps[i_step].k - kmp_steps[i_step].l);
    int next_x_l = x_l;
    if (i_step < kmp_steps.size())
    {
        next_x_l = settings.p.x() + settings.cell_size * (kmp_steps[i_step+1].k - kmp_steps[i_step+1].l);
    }
    if (i_step < kmp_steps.size() && next_x_l != x_l)
    {
        for (int i = next_x_l; i >= x_l; --i)
        {
            Demo::wait(2);
            drawKMP(painter,i);
            repaint();
        }
    }
    else
    {
        drawKMP(painter,x_l);
    }
}

void PictureBox::do_step(bool next)
{
    QPainter painter(&m_Pixmap);
    if (current_mode == DemoMode::drawPrefFunc)
    {
        drawPrefFunc(painter);   // -- отрисовываю текущий шаг -- //
    }
    else if (current_mode == DemoMode::drawPrefFunc_result)
    {
        drawPrefFunc_result(painter);   // -- отрисовываю текущий шаг -- //
    }
    else if (current_mode == DemoMode::drawKMP)
    {
        if (next)
        {
            do_next_step_kmp(painter);
        }
        else
        {
            do_prev_step_kmp(painter);
        }
    }
}
