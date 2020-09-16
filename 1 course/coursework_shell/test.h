#ifndef TEST_H
#define TEST_H

#include <array>

using namespace std;

struct Answer
{
    string text;
    bool right = false;
};

struct Question
{
    string text;
    array<Answer, 4> arr;
};

void creation_quest (vector<Question> &Questions);
int rand_answ (int ostatok);
void mixing_answers (vector <Question> &Questions);
void mixing_questions (vector <Question> &Questions);
void display_question (const Question &question, int number);
void display_test (const vector<Question> &Questions);
void test_results (int right, int wrong, int skip, unsigned int time);
void answer_result (const Question &question, bool right_or_not);
void test_menu ();

#endif // TEST_H
