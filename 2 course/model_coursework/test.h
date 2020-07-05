#ifndef TEST_H
#define TEST_H

#include <QWidget>
#include <array>
#include <vector>
#include <QKeyEvent>
#include <string>

namespace Ui {
class Test;
}

class Test : public QWidget
{
    Q_OBJECT
    enum quest_mode : uint8_t  // -- тип вопроса -- //
    {
        closed_answer = 1,
        open_answer = 2
    };

    struct Answer // -- ответ -- //
    {
        QString text = "";
        bool right = false;
    };
    struct user_answers // -- ответы пользователя -- //
    {
        std::vector<int> num;
        std::vector<QString> str;
    };

    struct Question // -- вопрос -- //
    {
        QString text = "";
        int mode = quest_mode::closed_answer;
        std::array<Answer, 4> arr;
    };


    std::string all_text;  // -- весь текст из файла -- //
    std::vector<Question> Questions;    // -- вопросы -- //
    user_answers user_choices;  // -- ответы юзера -- //
    size_t N = 0;   // -- кол-во вопросов -- //
    size_t i_step = 0;  // -- текущий отображаемый вопрос -- //
public:
    explicit Test(QWidget *parent = nullptr);
    ~Test();

signals:
    void return_to_menu();
    void enableMaxButtonOnWindow();
    void disableMaxButtonOnWindow();

private slots:
    void on_next_clicked(); // -- к следующему вопросу -- //
    void on_prev_clicked(); // -- к предыдущему -- //
    void on_Answer1_checkBox_clicked(); // -- нажатия на чекбоксы -- //
    void on_Answer2_checkBox_clicked();
    void on_Answer3_checkBox_clicked();
    void on_Answer4_checkBox_clicked();
    void on_finish_clicked();   // -- завершить тест -- //
    void on_answer_textEdited(const QString &arg1); // -- обработка ввода юзера - ответа юзера на вопрос с кратким ответом -- //
    void on_spinBox_valueChanged(int arg1); // -- счетчик для установки кол-ва вопросов перед тестом -- //
    void on_pushButton_start_clicked(); // -- начать -- //
    void on_return_to_menu_button_clicked();    // -- возврат в меню -- //
    void on_return_to_menu_button_2_clicked();
    void on_return_to_menu_button_3_clicked();
    //void on_pushButton_stats_clicked();   // -- посмотреть статистику -- //

    void on_pushButton_results_clicked();

private:
    Ui::Test *ui;
    void start_quest(); // -- начинаем тест -- //
    void creation_quest(); // -- формируем и рандомим вопросы -- //
    void display_question();    // -- вывод вопроса -- //
    void disable_checkBoxes();  // -- выключить все чекбоксы -- //
    void enable_checkBox(); // -- включить тот, на который ответил юзер (когда смотрим предыдущие вопросы) -- //
    void check_answers();   // -- проверка ответов -- //
    void keyPressEvent(QKeyEvent *event);   // -- обработка клавиш -- //
    void test_results(int right);   // -- вывод результатов теста -- //
    void add_stat(double procent);  // -- сохранить результаты в файл со статистикой -- //
    void read_stat();   // -- прочитать и вывести статистику из файла -- //
};

#endif // TEST_H
