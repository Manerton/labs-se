#include <QPainter>
#include <QKeyEvent>
#include "demo.h"
#include "ui_demo.h"
#include "kmp.h"

Demo::Demo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Demo)
{
    ui->setupUi(this);
    code.push_back("если a[i] != a[j], тогда, если j = 0, то Pi[i] = 0; ++i;");
    code.push_back("                          иначе (т.е j != 0) j = Pi[j-1];");
    code.push_back("иначе (т.е a[i] = a[j]) Pi[i] = j+1; ++i; ++j;");

    code.push_back("если t[k] != a[l], тогда, если l = 0, ++k и если k = n, то образа в строке нет;");
    code.push_back("                          иначе (т.е l != 0) l = Pi[l-1];");
    code.push_back("иначе (т.е t[k] = a[l]) ++k; ++l; если l = m, то образ найден");
}

Demo::~Demo()
{
    delete ui;
}

void Demo::on_pushButton_start_clicked()
{
    if (pb_frame != nullptr) delete pb_frame;
    pb_frame = new PictureBox(ui->frame);                            // -- инициализирую демонстрацию -- //
    if (ui->lineEdit_str->text().length() == 0)
    {
        pb_frame->str = "abcabeabcabcabd";
    }
    else
    {
        pb_frame->str = ui->lineEdit_str->text();
    }
    if (ui->lineEdit_substr->text().length() == 0)
    {
        pb_frame->substr = "abcabd";
    }
    else
    {
        pb_frame->substr = ui->lineEdit_substr->text();
    }
    ui->pushButton_result->setVisible(false);
    ui->pushButton_step2->setVisible(false);
    pb_frame->current_mode = PictureBox::DemoMode::drawPrefFunc;
    kmp algo(pb_frame->str,pb_frame->substr);                                           // -- вызываю алгоритм -- //
    pb_frame->Pi = algo.prefix_function();                          // -- префикс функция -- //
    pb_frame->pref_steps = algo.get_pref_step_info();               // -- получаю все шаги работы префикс функции -- //
    algo.kmp_algorithm(pb_frame->Pi);
    pb_frame->kmp_steps = algo.get_kmp_step_info();
    pb_frame->settings = pb_frame->prepare_drawSettings(pb_frame->substr.length());
    pb_frame->N_step = pb_frame->pref_steps.size();
    pb_frame->do_step(true);                                        // -- рисую шаг алгоритма -- //
    update_code();
    pb_frame->repaint();
    ui->stackedWidget->setCurrentWidget(ui->demonstration);
    ui->label_3->setText("Этап 1. Формирование массива префикс-функции");

}

void Demo::on_pushButton_result_clicked()
{
    pb_frame->current_mode = PictureBox::DemoMode::drawPrefFunc_result;
    pb_frame->i_step = 1;
    pb_frame->N_step = pb_frame->Pi.size();
    pb_frame->do_step(true);
    ui->label_3->setText("Результаты первого этапа");
    pb_frame->repaint();
}

void Demo::on_pushButton_step2_clicked()
{
    pb_frame->current_mode = PictureBox::DemoMode::drawKMP;
    pb_frame->i_step = 1;
    pb_frame->settings = pb_frame->prepare_drawSettings(pb_frame->str.length());
    pb_frame->N_step = pb_frame->kmp_steps.size();
    pb_frame->do_step(true);
    ui->label_3->setText("Этап 2. Непосредственный поиск подстроки в строке");
    pb_frame->repaint();
}

void Demo::update_code()
{
    PictureBox::DemoMode mode = pb_frame->current_mode;
    ui->textBrowser->clear();
    int i_step = int(pb_frame->i_step);
    int highlighted_line_i = 0;
    int start(0), end(0);
    if (mode == PictureBox::DemoMode::drawPrefFunc)
    {
        start = 0; end = 3;
        highlighted_line_i = pb_frame->pref_steps[size_t(i_step)].code_i; 
    }
    else if (mode == PictureBox::DemoMode::drawKMP)
    {
        start = 3; end = 6;
        highlighted_line_i = pb_frame->kmp_steps[size_t(i_step)].code_i;
    }
    for (int i = start ; i < end; ++i)
    {
        if (i == highlighted_line_i)
        {
            ui->textBrowser->setTextBackgroundColor(Qt::blue);
            ui->textBrowser->setTextColor(Qt::white);
        }
        ui->textBrowser->append(code[size_t(i)]);
        if (ui->textBrowser->textColor() != Qt::black)
        {
            ui->textBrowser->setTextBackgroundColor(Qt::white);
            ui->textBrowser->setTextColor(Qt::black);
        }
        ui->textBrowser->append("");
    }


}

void Demo::keyPressEvent(QKeyEvent *event) {
    if (event->key()==Qt::Key_Right) {
        if (pb_frame->i_step < pb_frame->N_step-1)
        {
            pb_frame->i_step++;
            update_code();
            pb_frame->do_step(true);
            pb_frame->repaint();
        }
        else if (pb_frame->i_step == pb_frame->N_step-1 || pb_frame->N_step == 1)
        {
            if (pb_frame->current_mode == PictureBox::DemoMode::drawPrefFunc)
            {
                ui->pushButton_result->setVisible(true);
            }
            else if (pb_frame->current_mode == PictureBox::DemoMode::drawPrefFunc_result)
            {
                ui->pushButton_step2->setVisible(true);
            }

        }
    }
    else if (event->key()==Qt::Key_Left) {
        if (pb_frame->i_step > 1)
        {
            pb_frame->i_step--;
            update_code();
            pb_frame->do_step(false);
            pb_frame->repaint();
        }
    }
}

void Demo::on_pushButton_finish_clicked()
{
    delete pb_frame;
    pb_frame = nullptr;
    ui->stackedWidget->setCurrentWidget(ui->menu);
    emit return_to_menu();
}

void Demo::on_lineEdit_str_textChanged(const QString &arg1)
{
    ui->lineEdit_substr->setMaxLength(arg1.length());
}
