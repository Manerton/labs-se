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
    enum quest_mode : uint8_t
    {
        closed_answer = 1,
        open_answer = 2
    };
    struct Answer
    {
        QString text = "";
        bool right = false;
    };
    struct user_answers
    {
        std::vector<int> num;
        std::vector<QString> str;
    };

    struct Question
    {
        QString text = "";
        int mode = quest_mode::closed_answer;
        std::array<Answer, 4> arr;
    };


    std::string all_text;
    std::vector<Question> Questions;
    user_answers user_choices;
    size_t N = 0;
    size_t i_step = 0;
public:
    explicit Test(QWidget *parent = nullptr);
    ~Test();

signals:
    void return_to_menu();

private slots:
    void on_next_clicked();
    void on_prev_clicked();
    void on_Answer1_checkBox_clicked();
    void on_Answer2_checkBox_clicked();
    void on_Answer3_checkBox_clicked();
    void on_Answer4_checkBox_clicked();
    void on_finish_clicked();
    void on_answer_textEdited(const QString &arg1);
    void on_spinBox_valueChanged(int arg1);
    void on_pushButton_start_clicked();
    void on_return_to_menu_button_clicked();
    void on_return_to_menu_button_2_clicked();
    void on_return_to_menu_button_3_clicked();
    void on_pushButton_results_clicked();

private:
    Ui::Test *ui;
    void start_quest();
    void creation_quest();
    void display_question();
    void disable_checkBoxes();
    void enable_checkBox();
    void check_answers();
    void keyPressEvent(QKeyEvent *event);           // -- обработка клавиш -- //
    void test_results(int right);
    void add_stat(double procent);
    void read_stat();
};

#endif // TEST_H
