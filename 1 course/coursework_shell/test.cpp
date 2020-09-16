#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <conio.h> // для getch()
#include <sstream> // строчный текстовый поток
#include "color.h" // цветной текст
#include "test.h"

void ShowTestHelp()
{
    system ("cls");
    SetColor(LightGreen,Black);
    cout << "Вы выбрали режим тестирования: \n" << endl;
    SetColor(LightGray,Black);
    cout << "Вам будет предложено 5 вопросов закрытого типа по теме \"Сортировка Шелла\"." << endl;
    cout << "Вы можете пропускать вопросы с возможностью вернуться к ним снова." << endl;
    cout << "На последнем вопросе вы можете завершить тестирование.\n" << endl;
    cout << "Нажмите цифру [1..4] для выбора варианта ответа на вопрос" << endl;
    cout << "Нажмите '->', для перехода к следующему вопросу" << endl;
    cout << "Нажмите '<-', для перехода к предыдущему вопросу" << endl;
    cout << "Нажмите 'enter', если вы хотите завершить тестирование\n" << endl;
    system("pause");
    system ("cls");
}

void add_stat (const vector<Question> &Questions, const vector<int> &ch, const int &right, const string &username)
{
    ofstream out("files\\stats.dat", ios::binary | ios::app);
    if(out)
    {
        string data;
        time_t td;
        td = time(NULL);
        data += ctime(&td);
        string temp;
        temp = "-----------------------------------------------\r\n";
        temp += "Итоги тестирования за: " + data;
        temp += "Пользователь: " + username + "\r\n";
        if (right !=5) temp += "Неправильные ответы:\r\n";
        else temp += "Неправильные ответы отсутствуют.\r\n";

        for (char &i : temp) i ^= 83; // зашифровываем строку
        out.write(temp.data(), temp.size());

        if (right != 5)
        {
            int N = ch.size();
            for (int i = 0; i < N; i++)
            {
                if (ch[i] == -1 || Questions[i].arr[ch[i]].right == false )
                {
                    temp = Questions[i].text;
                    temp += "\r\n";
                    for (char &i : temp) i ^= 83; // зашифровываем строку
                    out.write(temp.data(), temp.size());
                }
            }
        }

    }
}

void out_stats ()
{
    ifstream in("files\\stats.dat", ios::binary);
    if(in)
    {
        string temp;
        while (getline(in, temp)) // читаем строку (она может быть произвольной длины)
        {
            if (!in.eof()) temp += "\n"; // добавляем обратно символ \n в шифр, так как getline копирует строку игнорируя \n
            for (char &i : temp) i ^= 83; // расшифровываем
            cout << temp;
        }
    }
}


///-------------Меню------------///
void test ()
{
    vector<Question> Questions;
    creation_quest(Questions);
    mixing_questions(Questions);
    mixing_answers(Questions);
    display_test(Questions);
}

void input_test_menu (int &punkt, bool &ready)
{
    int button = getch();
    if (button == 224) button = getch();
    switch(button)
    {
    case '1':
        punkt = 3;
        ready = true;
        break;
    case '2':
        punkt = 4;
        ready = true;
        break;
    case '3':
        punkt = 5;
        ready = true;
        break;
    case 80:
        if (punkt != 5) punkt++;
        break;
    case 72:
        if (punkt != 3) punkt--;
        break;
    case 13:
        ready = true;
        break;
    default:
        input_test_menu(punkt, ready);
        break;
    }
}

void display_test_menu (const int &punkt)
{
    gotoXY(0,0);
    string blank(38, ' ');
    vector<string>vMenu = {
        "------------------------------------",
        "|-------- Меню тестирования -------|",
        "------------------------------------",
        "|1. Начать тестирование            |",
        "|2. Посмотреть статистику          |",
        "|3. Выйти в главное меню           |",
        "------------------------------------"
    };
    cout << "\n\n\n\n\n\n\n\n";
    int N = vMenu.size();
    for (int i = 0; i < N; i++)
    {
        if (i == punkt || i == 1)
        {
            int len = vMenu[i].length();
            cout << blank << vMenu[i][0];
            SetColor(White,Green);
            for (int j = 1; j < len-1; j++) cout << vMenu[i][j];
            SetColor(LightGray,Black);
            cout << vMenu[i][len-1] << endl;
        } else cout << blank << vMenu[i] << endl;
    }
}

void test_menu ()
{
    int punkt_menu = 3;
    while (punkt_menu != 5)
    {
        hide_cursor(false);
        display_test_menu(punkt_menu); // вывод меню в первый раз
        bool ready = false;
        while (!ready)
        {
            input_test_menu(punkt_menu, ready); // выбор пункта меню
            display_test_menu(punkt_menu); // подсветка выбора пункта
        }
        system("cls");
        switch (punkt_menu)
        {
        case 3:
            ShowTestHelp();
            test();
            break;
        case 4:
            set_console_buffer_size(1000);
            out_stats();
            system("pause");
            system("cls");
            set_console_buffer_size(27);
            break;
        }
    }
}

void input_test_menu2 (int &punkt, bool &ready)
{
    int button = getch();
    if (button == 224) button = getch();
    switch(button)
    {
    case '1':
        punkt = 3;
        ready = true;
        break;
    case '2':
        punkt = 4;
        ready = true;
        break;
    case 80:
        if (punkt != 4) punkt++;
        break;
    case 72:
        if (punkt != 3) punkt--;
        break;
    case 13:
        ready = true;
        break;
    default:
        input_test_menu2(punkt, ready);
        break;
    }
}

void display_test_menu2 (const int &punkt)
{
    gotoXY(0,0);
    string blank(38, ' ');
    vector<string>vMenu = {
        "------------------------------------",
        "|-------- Меню тестирования -------|",
        "------------------------------------",
        "|1. Посмотреть правильные ответы   |",
        "|2. Вернуться в главное меню       |",
        "------------------------------------"
    };
    cout << "\n\n\n\n\n\n\n\n";
    int N = vMenu.size();
    for (int i = 0; i < N; i++)
    {
        if (i == punkt || i == 1)
        {
            int len = vMenu[i].length();
            cout << blank << vMenu[i][0];
            SetColor(White,Green);
            for (int j = 1; j < len-1; j++) cout << vMenu[i][j];
            SetColor(LightGray,Black);
            cout << vMenu[i][len-1] << endl;
        } else cout << blank << vMenu[i] << endl;
    }
}

bool test_menu2 ()
{
    int punkt_menu = 3;
    while (punkt_menu != 4)
    {
        hide_cursor(false);
        display_test_menu2(punkt_menu); // вывод меню в первый раз
        bool ready = false;
        while (!ready)
        {
            input_test_menu2(punkt_menu, ready); // выбор пункта меню
            display_test_menu2(punkt_menu); // подсветка выбора пункта
        }
        system("cls");
        switch (punkt_menu)
        {
        case 3:
            return true;
            break;
        }
    }
    return false;
}
///---------Конец меню------------///


// Чтение вопросов из файла
void creation_quest (vector<Question> &Questions){
    ifstream input("files\\quest.dat");
    if(input)
    {
        string temp_string; // текущая строка
        string all_string; // весь текст в одной строке
        Question temp_question;
        while (getline(input, temp_string)) // расшифровываю текст из зашифрованного файла и записываю его в общую строку
        {
            if (!input.eof()) temp_string += "\n"; // вставляю \n так как Getline его не захватил
            for (char& i : temp_string) i ^= 83; // расшифровываю
            all_string += temp_string;
        }

        stringstream ss(all_string); // создаю текстовый поток из all_string
        while (getline(ss, temp_string)) // делю общую строку на строки (так как в общей строке были символы \n)
        {
            for (int i=0; i < 4; i++)
            {
                if (i == 0)
                {
                    temp_question.text = temp_string;
                    temp_question.arr[i].right = true;
                }
                getline(ss, temp_string);
                temp_question.arr[i].text = temp_string;
            }
            Questions.push_back(temp_question);
        }
    } else cout <<"Файл не обнаружен!" << endl;

}

int rand_answ (int ostatok){
    int result = rand() % ostatok;
    return result;
}

// Перемешивание дистракторов
void mixing_answers (vector <Question> &Questions)
{
    int N = Questions.size();
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int r = j + rand() % (4 - j);
            swap(Questions[i].arr[j], Questions[i].arr[r]);
        }
    }
}

// Перемешивание вопросов
void mixing_questions (vector <Question> &Questions)
{
    int N = Questions.size();
    for (int i = 0; i < N; i++)
    {
        int r = i + rand() % (N - i);
        swap(Questions[i], Questions[r]);
    }
}

// Вывод вопроса и подсветка ответа пользователя (в случае если showright = true, то и подсветка правильных ответов теста)
void display_question (const Question &question, const int &ch, const int &number, bool showright)
{
    SetColor(LightCyan,Black);
    cout << "Вопрос " << number + 1 <<": "<< question.text << endl;
    SetColor(LightGray,Black);
    for (int i = 0; i < 4; i++)
    {
        cout << i+1 << ") ";
        if (i == ch)
        {
            if (question.arr[ch].right == true) SetColor(White,Green);
            if (question.arr[ch].right == false) SetColor(LightGray,Red);
            cout << question.arr[i].text << endl;
            SetColor(LightGray,Black);
        } else if (showright && question.arr[i].right == true)
        {
            SetColor(White,Green);
            cout << question.arr[i].text << endl;
            SetColor(LightGray,Black);

        } else cout << question.arr[i].text << endl;
    }
}

// Результаты тестирования
void test_results (int right, int wrong, int skip, unsigned int time)
{
    if (right == 5)
    {
        SetColor(Green,Black);
        cout << "*** Поздравляю! ***"<< endl;
        SetColor(LightGray,Black);
        cout <<"Вы ответили на все вопросы правильно!"<<endl;
        cout <<"Ваша оценка: 5. ";
        SetColor(LightGreen,Black);
        cout << "Отлично! /Зачтено/" << endl;
        SetColor(LightGray,Black);
        cout <<"Вы затратили на это "<<time/1000.0<<" секунд" << endl;
    }
    if (right == 4)
    {
        cout << "Вы ответили на 4 вопроса правильно!"<<endl;
        if (skip != 0) cout << "Пропущенных вопросов: " << skip << endl;
        cout <<"Ваша оценка: 4. ";
        SetColor(LightGreen,Black);
        cout << "Хорошо! /Зачтено/" << endl;
        SetColor(LightGray,Black);
        cout <<"Вы затратили на это "<<time/1000.0<<" секунд" << endl;
    }
    if (right == 3)
    {
        cout <<"Вы ответили на 3 вопроса правильно!"<<endl;
        if (skip != 0) cout << "Пропущенных вопросов: " << skip << endl;
        cout <<"Ваша оценка: 3. ";
        SetColor(LightRed,Black);
        cout << "Удовлетворительно. /Зачтено/" << endl;
        SetColor(LightGray,Black);
        cout <<"Прочтите теорию заново!"<<endl;
    }
    if (right < 3)
    {
        cout << "Неправильных ответов: " << wrong << endl;
        if (skip != 0) cout << "Пропущенных вопросов: " << skip << endl;
        cout <<"Ваша оценка: 2. ";
        SetColor(LightRed,Black);
        cout << "Плохо! /Незачет/" << endl;
        SetColor(LightGray,Black);
        cout <<"Совет: внимательно ознакомиться с теорией и демонстрацией!"<<endl;
    }
}

// Ввод ответа пользователем
void input_test (int &ch, int &i, const int &size, bool &ready)
{
    int button = getch();
    if (button == 224) button = getch();
    if (ch == -1) // если ответ еще не был дан
    {
        switch(button)
        {
        case '1':
            ch = 0;
            break;
        case '2':
            ch = 1;
            break;
        case '3':
            ch = 2;
            break;
        case '4':
            ch = 3;
            break;
        case 77:
            if (i != size-1) i++;
            else input_test(ch, i, size, ready);
            break;
        case 75:
            if (i != 0) i--;
            else input_test(ch, i, size, ready);
            break;
        case 13:
            if (i == size-1) ready = true;
            else input_test(ch, i, size, ready);
            break;
        default:
            input_test(ch, i, size, ready);
            break;
        }
    } else { // если пользователь уже ответил на вопрос
        switch(button)
        {
        case 77:
            if (i != size-1) i++;
            else input_test(ch, i, size, ready);
            break;
        case 75:
            if (i != 0) i--;
            else input_test(ch, i, size, ready);
            break;
        case 13:
            if (i == size-1) ready = true;
            else input_test(ch, i, size, ready);
            break;
        default:
            input_test(ch, i, size, ready);
            break;
        }
    }
}

// Подсчет результатов
void check_answers (const vector<Question> &Questions, const vector<int> &ch, int &right, int &wrong, int &skip)
{
    int N = ch.size();
    for (int i = 0; i < N; i++)
    {
        if (ch[i] != -1)
        {
            if (Questions[i].arr[ch[i]].right == true) right++;
            else wrong++;
        } else skip++;
    }
}

// Вывод теста
void display_test (const vector<Question> &Questions)
{
    string username;
    cout << "Введите имя пользователя: ";
    cin >> username;
    int i = 0;
    int size = Questions.size();
    size = 5; // 5 вопросов
    vector<int> ch(size,-1);
    unsigned int start_time = clock();
    bool ready = false;
    while (!ready)
    {
        display_question(Questions[i], ch[i], i, false); // Вывод вопроса
        cout <<"\nВыберите правильный ответ нажатием соответствующей клавиши на клавиатуре."<<endl;
        if (i != size-1)
            cout << "Нажмите '->' для перехода к следующему вопросу." << endl;
        if (i != 0)
            cout << "Нажмите '<-' для возврата к предыдущему вопросу." << endl;
        if (i == size-1)
            cout << "Нажмите 'Enter' для завершения тестирования." << endl;
        input_test(ch[i],i,size,ready);
        system("cls");
    }
    unsigned int end_time = clock();
    unsigned int time = end_time - start_time;
    int right = 0, wrong = 0, skip = 0;
    check_answers(Questions, ch, right, wrong, skip);
    test_results(right,wrong,skip, time);
    system("pause");
    system("cls");
    if (test_menu2())
    {
        i = 0;
        ready = false;
        while (!ready)
        {
            display_question(Questions[i], ch[i], i, true); // Вывод правильных ответов
            if (i != size-1)
                cout << "Нажмите '->' для перехода к следующему вопросу." << endl;
            if (i != 0)
                cout << "Нажмите '<-' для возврата к предыдущему вопросу." << endl;
            if (i == size-1)
                cout << "Нажмите 'Enter' для того, чтобы выйти в меню." << endl;
            input_test(ch[i],i,size,ready);
            system("cls");
        }
    }
    add_stat(Questions, ch, right, username);
}
