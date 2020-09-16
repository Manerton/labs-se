#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <conio.h> // ��� getch()
#include <sstream> // �������� ��������� �����
#include "color.h" // ������� �����
#include "test.h"

void ShowTestHelp()
{
    system ("cls");
    SetColor(LightGreen,Black);
    cout << "�� ������� ����� ������������: \n" << endl;
    SetColor(LightGray,Black);
    cout << "��� ����� ���������� 5 �������� ��������� ���� �� ���� \"���������� �����\"." << endl;
    cout << "�� ������ ���������� ������� � ������������ ��������� � ��� �����." << endl;
    cout << "�� ��������� ������� �� ������ ��������� ������������.\n" << endl;
    cout << "������� ����� [1..4] ��� ������ �������� ������ �� ������" << endl;
    cout << "������� '->', ��� �������� � ���������� �������" << endl;
    cout << "������� '<-', ��� �������� � ����������� �������" << endl;
    cout << "������� 'enter', ���� �� ������ ��������� ������������\n" << endl;
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
        temp += "����� ������������ ��: " + data;
        temp += "������������: " + username + "\r\n";
        if (right !=5) temp += "������������ ������:\r\n";
        else temp += "������������ ������ �����������.\r\n";

        for (char &i : temp) i ^= 83; // ������������� ������
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
                    for (char &i : temp) i ^= 83; // ������������� ������
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
        while (getline(in, temp)) // ������ ������ (��� ����� ���� ������������ �����)
        {
            if (!in.eof()) temp += "\n"; // ��������� ������� ������ \n � ����, ��� ��� getline �������� ������ ��������� \n
            for (char &i : temp) i ^= 83; // ��������������
            cout << temp;
        }
    }
}


///-------------����------------///
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
        "|-------- ���� ������������ -------|",
        "------------------------------------",
        "|1. ������ ������������            |",
        "|2. ���������� ����������          |",
        "|3. ����� � ������� ����           |",
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
        display_test_menu(punkt_menu); // ����� ���� � ������ ���
        bool ready = false;
        while (!ready)
        {
            input_test_menu(punkt_menu, ready); // ����� ������ ����
            display_test_menu(punkt_menu); // ��������� ������ ������
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
        "|-------- ���� ������������ -------|",
        "------------------------------------",
        "|1. ���������� ���������� ������   |",
        "|2. ��������� � ������� ����       |",
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
        display_test_menu2(punkt_menu); // ����� ���� � ������ ���
        bool ready = false;
        while (!ready)
        {
            input_test_menu2(punkt_menu, ready); // ����� ������ ����
            display_test_menu2(punkt_menu); // ��������� ������ ������
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
///---------����� ����------------///


// ������ �������� �� �����
void creation_quest (vector<Question> &Questions){
    ifstream input("files\\quest.dat");
    if(input)
    {
        string temp_string; // ������� ������
        string all_string; // ���� ����� � ����� ������
        Question temp_question;
        while (getline(input, temp_string)) // ������������� ����� �� �������������� ����� � ��������� ��� � ����� ������
        {
            if (!input.eof()) temp_string += "\n"; // �������� \n ��� ��� Getline ��� �� ��������
            for (char& i : temp_string) i ^= 83; // �������������
            all_string += temp_string;
        }

        stringstream ss(all_string); // ������ ��������� ����� �� all_string
        while (getline(ss, temp_string)) // ���� ����� ������ �� ������ (��� ��� � ����� ������ ���� ������� \n)
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
    } else cout <<"���� �� ���������!" << endl;

}

int rand_answ (int ostatok){
    int result = rand() % ostatok;
    return result;
}

// ������������� ������������
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

// ������������� ��������
void mixing_questions (vector <Question> &Questions)
{
    int N = Questions.size();
    for (int i = 0; i < N; i++)
    {
        int r = i + rand() % (N - i);
        swap(Questions[i], Questions[r]);
    }
}

// ����� ������� � ��������� ������ ������������ (� ������ ���� showright = true, �� � ��������� ���������� ������� �����)
void display_question (const Question &question, const int &ch, const int &number, bool showright)
{
    SetColor(LightCyan,Black);
    cout << "������ " << number + 1 <<": "<< question.text << endl;
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

// ���������� ������������
void test_results (int right, int wrong, int skip, unsigned int time)
{
    if (right == 5)
    {
        SetColor(Green,Black);
        cout << "*** ����������! ***"<< endl;
        SetColor(LightGray,Black);
        cout <<"�� �������� �� ��� ������� ���������!"<<endl;
        cout <<"���� ������: 5. ";
        SetColor(LightGreen,Black);
        cout << "�������! /�������/" << endl;
        SetColor(LightGray,Black);
        cout <<"�� ��������� �� ��� "<<time/1000.0<<" ������" << endl;
    }
    if (right == 4)
    {
        cout << "�� �������� �� 4 ������� ���������!"<<endl;
        if (skip != 0) cout << "����������� ��������: " << skip << endl;
        cout <<"���� ������: 4. ";
        SetColor(LightGreen,Black);
        cout << "������! /�������/" << endl;
        SetColor(LightGray,Black);
        cout <<"�� ��������� �� ��� "<<time/1000.0<<" ������" << endl;
    }
    if (right == 3)
    {
        cout <<"�� �������� �� 3 ������� ���������!"<<endl;
        if (skip != 0) cout << "����������� ��������: " << skip << endl;
        cout <<"���� ������: 3. ";
        SetColor(LightRed,Black);
        cout << "�����������������. /�������/" << endl;
        SetColor(LightGray,Black);
        cout <<"�������� ������ ������!"<<endl;
    }
    if (right < 3)
    {
        cout << "������������ �������: " << wrong << endl;
        if (skip != 0) cout << "����������� ��������: " << skip << endl;
        cout <<"���� ������: 2. ";
        SetColor(LightRed,Black);
        cout << "�����! /�������/" << endl;
        SetColor(LightGray,Black);
        cout <<"�����: ����������� ������������ � ������� � �������������!"<<endl;
    }
}

// ���� ������ �������������
void input_test (int &ch, int &i, const int &size, bool &ready)
{
    int button = getch();
    if (button == 224) button = getch();
    if (ch == -1) // ���� ����� ��� �� ��� ���
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
    } else { // ���� ������������ ��� ������� �� ������
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

// ������� �����������
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

// ����� �����
void display_test (const vector<Question> &Questions)
{
    string username;
    cout << "������� ��� ������������: ";
    cin >> username;
    int i = 0;
    int size = Questions.size();
    size = 5; // 5 ��������
    vector<int> ch(size,-1);
    unsigned int start_time = clock();
    bool ready = false;
    while (!ready)
    {
        display_question(Questions[i], ch[i], i, false); // ����� �������
        cout <<"\n�������� ���������� ����� �������� ��������������� ������� �� ����������."<<endl;
        if (i != size-1)
            cout << "������� '->' ��� �������� � ���������� �������." << endl;
        if (i != 0)
            cout << "������� '<-' ��� �������� � ����������� �������." << endl;
        if (i == size-1)
            cout << "������� 'Enter' ��� ���������� ������������." << endl;
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
            display_question(Questions[i], ch[i], i, true); // ����� ���������� �������
            if (i != size-1)
                cout << "������� '->' ��� �������� � ���������� �������." << endl;
            if (i != 0)
                cout << "������� '<-' ��� �������� � ����������� �������." << endl;
            if (i == size-1)
                cout << "������� 'Enter' ��� ����, ����� ����� � ����." << endl;
            input_test(ch[i],i,size,ready);
            system("cls");
        }
    }
    add_stat(Questions, ch, right, username);
}
