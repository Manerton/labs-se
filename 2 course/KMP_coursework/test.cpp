#include <sstream>
#include <ctime>
#include <QMessageBox>
#include <random>
#include "test.h"
#include "ui_test.h"
#include "loader.h"

using namespace std;


Test::Test(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Test)
{
    ui->setupUi(this);
    all_text = loader::getText_from_file("files\\quest.txt");
    if (all_text.length() == 0) // -- лоадер вернул пустую строку, значит не смог открыть файл -- //
    {
        QMessageBox::warning(this,"Ошибка","Не удалось открыть файл files\\quest.txt");
        ui->pushButton_start->setDisabled(true);
    }
    N = 5;  // -- по умолчанию 5 вопросов -- //
}

void Test::start_quest()
{
    time_t seed = time(nullptr);
    mt19937 mt(seed);
    // -- очищаем все если тест проходим не в первый раз -- //
    if (Questions.size() > 0 ) Questions.clear();
    if (user_choices.num.size() > 0) user_choices.num.clear();
    if (user_choices.str.size() > 0) user_choices.str.clear();
    if (ui->listWidget_result->count() > 0) ui->listWidget_result->clear();
    if (i_step != 0) i_step = 0;
    creation_quest();  // -- формируем вопросы -- //
    shuffle(Questions.begin(),Questions.end(),mt);  // -- мешаем вопросы -- //
    for (Question &quest : Questions)
    {
        if (quest.mode == quest_mode::closed_answer)
        {
            shuffle(quest.arr.begin(),quest.arr.end(),mt);  // -- мешаем ответы, если тип вопроса - закрытый -- //
        }
    }
    display_question(); // -- выводим -- //
}

void Test::creation_quest()
{
    string temp_string; // текущая строка
    Question temp_question;
    stringstream ss(all_text); // создаю текстовый поток из all_text
    while (getline(ss, temp_string))
    {
        temp_question.mode = temp_string[1] - '0';  // -- читаю тип вопроса -- //
        temp_question.text = QString::fromStdString({temp_string.begin()+2, temp_string.end()});    // -- читаю текст вопроса -- //
        switch (temp_question.mode)
        {
            case quest_mode::closed_answer:

                temp_question.arr[0].right = true;  // -- первый вариант ответа изначально правильный -- //
                for (size_t i=0; i < 4; ++i)
                {
                    getline(ss, temp_string);   // -- читаю варианты ответов -- //
                    temp_question.arr[i].text = QString::fromStdString(temp_string);
                }
                user_choices.num.resize(N,-1);  // -- отмечаю пока ответы как -1, что означает, что ответа пользователь еще не дал -- //
                break;
            case quest_mode::open_answer:
                getline(ss, temp_string);
                temp_question.arr[0].text = QString::fromStdString(temp_string).toLower();
                user_choices.str.resize(N,"");  // -- а тут пустые строки, что означает, что ответ юзер не дал -- //
                break;
        }
        Questions.push_back(temp_question);
    }
}

void Test::display_question()
{
    ui->textBrowser->setText(Questions[i_step].text);
    switch (Questions[i_step].mode)
    {
        case quest_mode::closed_answer:
            ui->stackedWidget_question_type->setCurrentWidget(ui->closed_question);
            ui->Answer1->setText(Questions[i_step].arr[0].text);    // -- устанавливаю текст для вариантов ответов -- //
            ui->Answer2->setText(Questions[i_step].arr[1].text);
            ui->Answer3->setText(Questions[i_step].arr[2].text);
            ui->Answer4->setText(Questions[i_step].arr[3].text);
            disable_checkBoxes();   // -- выключаю старые чекбоксы -- //
            enable_checkBox();  // -- включаю тот, который юзер отметил, если еще не отметил, то ничего не включаю -- //
            break;
        case quest_mode::open_answer:
            ui->stackedWidget_question_type->setCurrentWidget(ui->open_question);
            ui->answer->setText(user_choices.str[i_step]);
            break;
    }
}

void Test::check_answers()
{
    int right(0);
    for (size_t i = 0; i < N; ++i)
    {
        int ch;
        QString str;
        ui->listWidget_result->addItem(Questions[i].text);
        switch (Questions[i].mode)
        {
            case quest_mode::closed_answer:
                ch = user_choices.num[i];

                if (ch != -1)
                {
                    if (Questions[i].arr[size_t(ch)].right)
                    {
                        ++right;
                        ui->listWidget_result->item(int(i))->setBackground(Qt::green);  // -- помечаю зеленым - правильный ответ -- //
                    }
                    else
                    {
                        ui->listWidget_result->item(int(i))->setBackground(Qt::red);    // -- помечаю красным - неправильный -- //
                        ui->listWidget_result->item(int(i))->setForeground(Qt::white);
                    }
                }
                break;
            case quest_mode::open_answer:
                str = user_choices.str[i];
                if (str != "")
                {
                    if (Questions[i].arr[0].text == str)
                    {
                        ++right;
                        ui->listWidget_result->item(int(i))->setBackground(Qt::green);
                    }
                    else
                    {
                        ui->listWidget_result->item(int(i))->setBackground(Qt::red);
                        ui->listWidget_result->item(int(i))->setForeground(Qt::white);
                    }
                }
                break;
        }
    }
   test_results(right);
}

void Test::test_results(int right)
{
    double procent = double(right) / N * 100;
    ui->label_proc->setText(QString::number(procent));
    if (procent >= 84.9)
    {
        ui->label_ocenka->setText(QString::number(5));
    }
    else if (procent >= 74.9)
    {
        ui->label_ocenka->setText(QString::number(4));
    }
    else if (procent >= 59.9)
    {
        ui->label_ocenka->setText(QString::number(3));
    }
    else
    {
        ui->label_ocenka->setText(QString::number(2));
    }
    add_stat(procent);
}

void Test::enable_checkBox()
{
    ui->Answer1_checkBox->setAutoExclusive(true);
    ui->Answer2_checkBox->setAutoExclusive(true);
    ui->Answer3_checkBox->setAutoExclusive(true);
    ui->Answer4_checkBox->setAutoExclusive(true);
    switch (user_choices.num[i_step])
    {
        case -1:
            break;
        case 0:
            ui->Answer1_checkBox->setChecked(true);
            break;
        case 1:
            ui->Answer2_checkBox->setChecked(true);
            break;
        case 2:
            ui->Answer3_checkBox->setChecked(true);
            break;
        case 3:
            ui->Answer4_checkBox->setChecked(true);
            break;
    }
}

void Test::add_stat (double procent)
{
    string filename = "files\\stats.txt";
    ofstream out(filename, ios::app);
    if(out)
    {
        time_t td;
        td = time(nullptr);
        QString temp;
        temp = "-----------------------------------------------\r\n";
        temp += "Итоги тестирования за: ";
        temp += ctime(&td);
        temp += "Пользователь: " + ui->username->text() + "\r\n";
        if (procent < 99.9) temp += "Неправильные ответы:\r\n";
        else temp += "Неправильные ответы отсутствуют.\r\n";

        if (procent < 99.9)
        {
            for (size_t i = 0; i < N; i++)
            {
                if (ui->listWidget_result->item(int(i))->background() != Qt::green)
                {
                    temp += ui->listWidget_result->item(int(i))->text() + "\r\n";
                }
            }
        }
        out << temp.toStdString();
    } else QMessageBox::warning(this,"Ошибка","Не удалось сохранить статистику в файл " + QString::fromStdString(filename));
}

void Test::read_stat ()
{
    QString stats_text = QString::fromStdString(loader::getText_from_file("files\\stats.txt"));
    ui->textBrowser_stats->setText(stats_text);
}

Test::~Test()
{
    delete ui;
}

void Test::on_next_clicked()
{
    if (i_step < size_t(N-1))
    {
        ++i_step;
        display_question();
    }
}

void Test::on_prev_clicked()
{
    if (i_step > 0)
    {
        --i_step;
        display_question();
    }
}

void Test::keyPressEvent(QKeyEvent *event) {
    if (event->key()==Qt::Key_Right) {
        on_next_clicked();
    }
    else if (event->key()==Qt::Key_Left) {
        on_prev_clicked();
    }
    else if (event->key()==Qt::Key_Return)
    {
        on_finish_clicked();
    }
}


void Test::disable_checkBoxes()
{
    ui->Answer1_checkBox->setAutoExclusive(false);
    ui->Answer2_checkBox->setAutoExclusive(false);
    ui->Answer3_checkBox->setAutoExclusive(false);
    ui->Answer4_checkBox->setAutoExclusive(false);
    ui->Answer1_checkBox->setChecked(false);
    ui->Answer2_checkBox->setChecked(false);
    ui->Answer3_checkBox->setChecked(false);
    ui->Answer4_checkBox->setChecked(false);
}

void Test::on_Answer1_checkBox_clicked()
{
    user_choices.num[i_step] = 0;
}
void Test::on_Answer2_checkBox_clicked()
{
    user_choices.num[i_step] = 1;
}
void Test::on_Answer3_checkBox_clicked()
{
    user_choices.num[i_step] = 2;
}
void Test::on_Answer4_checkBox_clicked()
{
    user_choices.num[i_step] = 3;
}


void Test::on_finish_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->result);
    check_answers();

}

void Test::on_answer_textEdited(const QString &arg1)
{
    user_choices.str[i_step] = arg1.toLower();
}

void Test::on_spinBox_valueChanged(int arg1)
{
    N = size_t(arg1);
}

void Test::on_pushButton_start_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->test);
    start_quest();
}

void Test::on_return_to_menu_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->mainmenu);
    emit return_to_menu();
}

void Test::on_return_to_menu_button_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->mainmenu);
    emit return_to_menu();
}

void Test::on_return_to_menu_button_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->mainmenu);
    emit return_to_menu();
}

void Test::on_pushButton_results_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->stats);
    read_stat();
}
